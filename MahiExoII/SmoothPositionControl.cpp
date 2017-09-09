#include "SmoothPositionControl.h"
#include "Input.h"
#include "mahiexoii_util.h"

SmoothPositionControl::SmoothPositionControl(mel::Clock& clock, mel::Daq* daq, mel::MahiExoII& meii) :
    StateMachine(5),
    clock_(clock),
    daq_(daq),
    meii_(meii)
{
}

void SmoothPositionControl::wait_for_input() {
    mel::Input::wait_for_key_press(mel::Input::Key::Space);
}

bool SmoothPositionControl::check_stop() {
    return mel::Input::is_key_pressed(mel::Input::Escape) || (mel::Input::is_key_pressed(mel::Input::LControl) && mel::Input::is_key_pressed(mel::Input::C));
}

//-----------------------------------------------------------------------------
// "INITIALIZATION" STATE FUNCTION
//-----------------------------------------------------------------------------
void SmoothPositionControl::sf_init(const mel::NoEventData* data) {

    // enable MEII EMG DAQ
    mel::print("\nPress Enter to enable MEII EMG Daq <" + daq_->name_ + ">.");
    mel::Input::wait_for_key_press(mel::Input::Key::Return);
    daq_->enable();
    if (!daq_->is_enabled()) {
        event(ST_STOP);
        return;
    }

    // check DAQ behavior for safety
    daq_->read_all();
    meii_.update_kinematics();
    if (meii_.check_all_joint_limits()) {
        event(ST_STOP);
        return;
    }
    if (!check_digital_loopback(0, 7)) {
        event(ST_STOP);
        return;
    }

    // enable MEII
    mel::print("\nPress Enter to enable MEII.");
    mel::Input::wait_for_key_press(mel::Input::Key::Return);
    meii_.enable();
    if (!meii_.is_enabled()) {
        event(ST_STOP);
        return;
    }

    // confirm start of experiment
    mel::print("\nPress Enter to run Position Control");
    mel::Input::wait_for_key_press(mel::Input::Key::Return);
    mel::print("\nRunning Position Control ... ");

    // start the watchdog
    daq_->start_watchdog(0.1);

    // start the clock
    clock_.start();

    // transition to next state
    if (stop_) {
        event(ST_STOP);
    }
    else {
        event(ST_TRANSPARENT);
    }
}


//-----------------------------------------------------------------------------
// "TRANSPARENT" STATE FUNCTION
//-----------------------------------------------------------------------------
void SmoothPositionControl::sf_transparent(const mel::NoEventData* data) {
    mel::print("Robot Transparent");

    // restart the clock
    clock_.start();

    // enter the control loop
    while (!init_transparent_time_reached_ && !stop_) {

        // read and reload DAQs
        daq_->reload_watchdog();
        daq_->read_all();

        // update robot kinematics
        meii_.update_kinematics();


        // check joint limits
        if (meii_.check_all_joint_limits()) {
            stop_ = true;
            break;
        }

        // set zero torques
        meii_.set_joint_torques({ 0.0, 0.0, 0.0, 0.0, 0.0 });

        // write to daq
        daq_->write_all();

        // check for init transparent time reached
        init_transparent_time_reached_ = check_wait_time_reached(init_transparent_time_, 0.0, clock_.time());

        // check for stop input
        stop_ = check_stop();

        // wait for the next clock cycle
        clock_.wait();
    }


    // transition to next state
    if (stop_) {
        // stop if user provided input
        event(ST_STOP);
    }
    else if (init_transparent_time_reached_) {
        start_pos_ = meii_.get_anatomical_joint_positions(); // set starting position as current position
        event(ST_WAYPOINT);
    }
    else {
        mel::print("ERROR: State transition undefined. Going to ST_STOP.");
        event(ST_STOP);
    }
}


//-----------------------------------------------------------------------------
// "WAYPOINT" STATE FUNCTION
//-----------------------------------------------------------------------------
void SmoothPositionControl::sf_waypoint(const mel::NoEventData* data) {
    mel::print("Moving to Waypoint");

    // restart the clock
    clock_.start();

    // initialize event variables
    waypoint_reached_ = false;

    // set goal position as next waypoint position
    goal_pos_ = wp_1_;

    // enter the control loop
    while (!waypoint_reached_ && !stop_) {

        // read and reload DAQs
        daq_->reload_watchdog();
        daq_->read_all();

        // update robot kinematics
        meii_.update_kinematics();

        // check joint limits
        if (meii_.check_all_joint_limits()) {
            stop_ = true;
            break;
        }

        switch (rps_control_mode_) {

        // robot joint space (parallel)
        case 0 : 

            // compute pd torques
            for (auto i = 0; i < 5; ++i) {
                ref_pos_[i] = moving_set_point(start_pos_[i], goal_pos_[i], 0.0, clock_.time(), speed_[i]);
            }
            std::copy(ref_pos_.begin() + 2, ref_pos_.end(), q_ser_ref_.begin());
            meii_.inverse_kinematics(q_ser_ref_, q_par_ref_);
            std::copy(q_par_ref_.begin(), q_par_ref_.end(), ref_pos_.begin() + 2);
            for (auto i = 0; i < 5; ++i) {
                //pd_torques_[i] = mel::pd_controller(kp_[i], kd_[i], ref_pos_[i], meii_.joints_[i]->get_position(), 0, meii_.joints_[i]->get_velocity());
                pd_torques_[i] = meii_.robot_joint_pd_controllers_[i].calculate(ref_pos_[i], meii_.joints_[i]->get_position(), 0, meii_.joints_[i]->get_velocity());
                if (robot_joint_backdrive_[i] == 1) {
                    commanded_torques_[i] = 0;
                }
                else {
                    commanded_torques_[i] = pd_torques_[i];
                }
            }

            // set command torques
            mel::print(pd_torques_);
            
            meii_.set_joint_torques(commanded_torques_);

            break;

        // anatomical joint space (serial)
        case 1 : 

            // compute pd torques
            for (auto i = 0; i < 5; ++i) {
                ref_pos_[i] = moving_set_point(start_pos_[i], goal_pos_[i], 0.0, clock_.time(), speed_[i]);
                pd_torques_[i] = meii_.anatomical_joint_pd_controllers_[i].calculate(ref_pos_[i], meii_.get_anatomical_joint_position(i), 0, meii_.get_anatomical_joint_velocity(i));
                if (anatomical_joint_backdrive_[i] == 1) {
                    commanded_torques_[i] = 0;
                }
                else {
                    commanded_torques_[i] = pd_torques_[i];
                }
            }

            // set command torques
            mel::print(pd_torques_);
            meii_.set_anatomical_joint_torques(commanded_torques_);

            break;
        }

        // write to daq
        daq_->write_all();

        // check for waypoint reached
        waypoint_reached_ = check_waypoint_reached(goal_pos_, meii_.get_anatomical_joint_positions(), check_joint_);

        // check for stop input
        stop_ = check_stop();

        // wait for the next clock cycle
        clock_.wait();
    }


    // transition to next state
    if (stop_) {
        // stop if user provided input
        event(ST_STOP);
    }
    else if (waypoint_reached_) {
        ++current_wp_;
        if (current_wp_ < num_wp_) {
            start_pos_ = goal_pos_; // set starting position as current waypoint
            event(ST_WAYPOINT);
        }
        else {
            event(ST_FINISH);
        }
    }
    else {
        mel::print("ERROR: State transition undefined. Going to ST_STOP.");
        event(ST_STOP);
    }
}

//-----------------------------------------------------------------------------
// "FINISH" STATE FUNCTION
//-----------------------------------------------------------------------------
void SmoothPositionControl::sf_finish(const mel::NoEventData* data) {
    mel::print("Finish Execution");

    if (meii_.is_enabled()) {
        meii_.disable();
    }
    if (daq_->is_enabled()) {
        daq_->disable();
    }

}

//-----------------------------------------------------------------------------
// "STOP" STATE FUNCTION
//-----------------------------------------------------------------------------

void SmoothPositionControl::sf_stop(const mel::NoEventData* data) {
    std::cout << "Stop Robot" << std::endl;
    if (meii_.is_enabled()) {
        meii_.disable();
    }
    if (daq_->is_enabled()) {
        daq_->disable();
    }
}

//-----------------------------------------------------------------------------
// UTILITY FUNCTIONS
//-----------------------------------------------------------------------------

bool SmoothPositionControl::check_waypoint_reached(mel::double_vec goal_pos, mel::double_vec current_pos, mel::char_vec target_check_joint, bool print_output) {

    bool target_reached = true;
    for (int i = 0; i < 5; ++i) {
        if (target_check_joint[i]) {
            if (std::abs(goal_pos[i] - current_pos[i]) > std::abs(pos_tol_[i])) {
                if (print_output && target_reached) {
                    std::cout << "Joint " << std::to_string(i) << " error is " << (abs(goal_pos[i] - current_pos[i])*mel::RAD2DEG) << std::endl;
                }
                target_reached = false;
            }
        }
    }
    return target_reached;
}

bool SmoothPositionControl::check_wait_time_reached(double wait_time, double init_time, double current_time) {
    return (current_time - init_time) > wait_time;
}
#pragma once
#include "StateMachine.h"
#include "Q8Usb.h"
#include "MahiExoII.h"
#include "util.h"
#include "mahiexoii_util.h"
#include "MelShare.h"
#include "Filter.h"

class EmgTrainingData : public mel::EventData {

public:

};

class EmgTraining : public mel::StateMachine {

public:

    EmgTraining(mel::Clock& clock, mel::Daq* q8_emg, mel::Daq* q8_ati, MahiExoII* exo);
    

    

private:
    
    enum States {
        ST_INIT,
        ST_TO_NEUTRAL,
        ST_HOLD_NEUTRAL,
        ST_STOP,
        ST_NUM_STATES
    };

    // Define the state machine state functions with event data type
    void ST_Init(const mel::NoEventData*);
    mel::StateAction<EmgTraining, mel::NoEventData, &EmgTraining::ST_Init> Init;

    void ST_To_Neutral(const mel::NoEventData*);
    mel::StateAction<EmgTraining, mel::NoEventData, &EmgTraining::ST_To_Neutral> To_Neutral;

    void ST_Hold_Neutral(const mel::NoEventData*);
    mel::StateAction<EmgTraining, mel::NoEventData, &EmgTraining::ST_Hold_Neutral> Hold_Neutral;

    void ST_Stop(const mel::NoEventData*);
    mel::StateAction<EmgTraining, mel::NoEventData, &EmgTraining::ST_Stop> Stop;

    // State map to define state object order. Each state map entry defines a
    // state object.
    virtual const mel::StateMapRow* get_state_map() {
        static const mel::StateMapRow STATE_MAP[] = {
            &Init,
            &To_Neutral,
            &Hold_Neutral,
            &Stop,
        };
        return &STATE_MAP[0];
    }

    void ctrl_c_task() override;


    mel::Daq* q8_emg_;
    mel::Daq* q8_ati_;
    MahiExoII* exo_;


    Eigen::VectorXd raw_force_;
    Eigen::MatrixXd calib_mat_;
    Eigen::VectorXd calib_force_;


    mel::double_vec set_points_;
    mel::double_vec kp_;
    mel::double_vec kd_;
    mel::double_vec init_pos_;
    mel::double_vec goal_pos_;
    double init_time_;
    mel::double_vec speed_;
    mel::double_vec x_ref_;
    mel::double_vec new_torques_;
    mel::int8_vec backdrive_;

    mel::Filter lpf_;
    mel::Filter multi_lpf_;

    // MELShare(s)
    mel::share::MelShare pos_data_ = mel::share::MelShare("MEII_pos");
    mel::double_vec data_p_;
    mel::share::MelShare ati_data_ = mel::share::MelShare("MEII_ati");
    mel::double_vec force_share_;
    mel::double_vec force_filt_;

};
#pragma once
#include "Actuator.h"

namespace mel {

    class Motor : public Actuator {

    public:

        // constructors
        Motor();
        Motor(double kt, double current_limit, double amp_gain, Daq::AoChannel ao_channel, Daq::DoChannel do_channel);

        void set_torque(double actuator_torque) override;
        void set_current(double current);
        double limit_current(double current_new);

        void enable() override;
        void disable() override;
        
        const double kt_;
        const double current_limit_;
        const double amp_gain_;
        bool is_enable_;
 

    private:
        
        Daq::AoChannel ao_channel_;
        Daq::DoChannel do_channel_;

        double current_;
        double limited_current_;
        

        

    };
}
#pragma once
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "util.h"
#include <boost/filesystem.hpp>

namespace mel {

    class Daq {

    public:

        // CONSTRUCTOR(S) / DESTRUCTOR(S)

        Daq(std::string name,
            channel_vec ai_channels,
            channel_vec ao_channels,
            channel_vec di_channels,
            channel_vec do_channels,
            channel_vec encoder_channels,
            channel_vec encrate_channels);

        // PUBLIC VARIABLES

        const std::string name_;

        // PURE VIRTUAL FUNCTIONS FOR ACTIVATING AND DEACTIVATING DAQ (REQ'D BY ALL DERIVED CLASSES)

        virtual int activate() = 0;
        virtual int deactivate() = 0;

        // VIRTUAL FUNCTIONS FOR IMPLEMENTING READING / WRITING FUNCTIONALITY

        virtual void read_analogs() { 
            std::cout << "WARNING: DAQ <" << name_ << "> does not implement read_analogs()"    << std::endl; }
        virtual void read_digitals() { 
            std::cout << "WARNING: DAQ <" << name_ << "> does not implement read_digitals()"   << std::endl; }
        virtual void read_encoders() { 
            std::cout << "WARNING: DAQ <" << name_ << "> does not implement read_encoders()"   << std::endl; }
        virtual void read_encrates() { 
            std::cout << "WARNING: DAQ <" << name_ << "> does not implement read_encrates()"   << std::endl; }
        virtual void read_all() { 
            std::cout << "WARNING: DAQ <" << name_ << "> does not implement read_all()"        << std::endl; }

        virtual void write_analogs() { 
            std::cout << "WARNING: DAQ <" << name_ << "> does not implement write_analogs()"   << std::endl; }
        virtual void write_digitals() { 
            std::cout << "WARNING: DAQ <" << name_ << "> does not implement write_digitals()"  << std::endl; }
        virtual void write_all() { 
            std::cout << "WARNING: DAQ <" << name_ << "> does not implement write_all()"       << std::endl; }

        virtual void zero_encoders()  { 
            std::cout << "WARNING: DAQ <" << name_ << "> does not implement zero_encoders()"   << std::endl; }
        virtual void offset_encoders(int32_vec offset_counts) { 
            std::cout << "WARNING: DAQ <" << name_ << "> does not implement offset_encoders()" << std::endl; }

        // VIRTUAL FUNCTIONS FOR IMPLEMENTING A WATCHDOG TIMER

        virtual void reload_watchdog() { 
            std::cout << "WARNING: DAQ <" << name_ << "> does not implement reload_watchdog()" << std::endl; }
        virtual void start_watchdog(double watchdog_timeout) { 
            std::cout << "WARNING: DAQ <" << name_ << "> does not implement start_watchdog()"  << std::endl; }
        virtual void stop_watchdog() { 
            std::cout << "WARNING: DAQ <" << name_ << "> does not implement stop_watchdog()"   << std::endl; }

        // PUBLIC FUNCTIONS

        void log_data(double timestamp);     // function to log all state information to data log file    


        // FUNCTIONS FOR GETTING/SETTING DAQ STATES (DEFAULT BEHAVIOR MAY BE OVERRIDDEN IF DESIRED)

        virtual voltage_vec get_analog_voltages();
        virtual voltage get_analog_voltage(channel channel_number);
        virtual void set_analog_voltages(voltage_vec new_voltages);
        virtual void set_analog_voltage(channel channel_number, voltage new_voltage);

        virtual dsignal_vec get_digital_signals();
        virtual dsignal get_digital_signal(channel channel_number);
        virtual void set_digital_signals(dsignal_vec new_signals);
        virtual void set_digital_signal(channel channel_number, dsignal new_signal);

        virtual int32_vec get_encoder_counts();
        virtual int32 get_encoder_count(channel channel_number);
        virtual double_vec get_encoder_rates();
        virtual double get_encoder_rate(channel channel_number);

        // FUNCTIONS FOR SETTING OPTIONS
        // TODO: PRESERVE SIZE
        virtual void set_ai_voltage_ranges(voltage_vec min_voltages, voltage_vec max_voltages) { ai_min_voltages_ = min_voltages; ai_max_voltages_ = max_voltages; }
        virtual void set_ao_voltage_ranges(voltage_vec min_voltages, voltage_vec max_voltages) { ao_min_voltages_ = min_voltages; ao_max_voltages_ = max_voltages; }

        virtual void set_ao_initial_voltages(voltage_vec initial_voltages) { ao_initial_voltages_ = initial_voltages; }
        virtual void set_ao_final_voltages(voltage_vec final_voltages) { ao_final_voltages_ = final_voltages;  }
        virtual void set_ao_expire_voltages(voltage_vec expire_voltages) { ao_expire_voltages_ = expire_voltages;  }

        virtual void set_do_initial_signals(dsignal_vec initial_signals) { do_initial_signals_ = initial_signals; }
        virtual void set_do_final_signals(dsignal_vec final_signals) { do_final_signals_ = final_signals; }
        virtual void set_do_expire_signals(dsignal_vec expire_signals) { do_expire_signals_ = expire_signals; }

        virtual void set_encoder_quadrature_factors(uint32_vec quadrature_factors) { encoder_quadrature_factors_ = quadrature_factors; }

        // FUNCTIONS FOR GETTING OPTIONS

        uint32_vec get_encoder_quadrature_factors();
        uint32 get_encoder_quadrature_factor(channel channel_number);

    protected:

        // DAQ DATA LOGGING

        const std::string   log_dir_ = "daq_logs"; // folder where data logs will be stored
        const std::string   data_log_filename_;    // filename of the data log
        std::ofstream data_log_;             // stream for logging to the data log file

        // STATE VARIABLES

        bool active_ = false;      // bool indicating whether the DAQ is currently active

        voltage_vec ai_voltages_;  // vector for which analog inputs should be read in to
        voltage_vec ao_voltages_;  // vector for which analog outputs should be written out from
        dsignal_vec di_signals_;   // vector for which digital inputs should be read in to
        dsignal_vec do_signals_;   // vector for which digital outputs should be written out from
        int32_vec   enc_counts_;   // vector for which encoder counts should be read in to
        double_vec  enc_rates;     // vector for which encoder rates should be read in to

        // CHANNEL NUMBERS BEING USED

        const channel_vec   ai_channel_nums_;       // vector of analog input channels numbers being used 
        const channel_vec   ao_channel_nums_;       // vector of analog output channels numbers being used 
        const channel_vec   di_channel_nums_;       // vector of digital input channels numbers being used 
        const channel_vec   do_channel_nums_;       // vector of digital output channels numbers being used 
        const channel_vec   encoder_channel_nums_;  // vector of encoder channels being numbers used
        const channel_vec   encrate_channel_nums_;  // vector of encoder velocity channels numbers being used

        // NUMBER OF CHANNELS DEFINED

        const size_t   num_ai_channels_;       // vector of analog input channels being used 
        const size_t   num_ao_channels_;       // vector of analog output channels being used 
        const size_t   num_di_channels_;       // vector of digital input channels being used 
        const size_t   num_do_channels_;       // vector of digital output channels being used 
        const size_t   num_encoder_channels_;  // vector of encoder channels being used
        const size_t   num_encrate_channels_;  // vector of encoder velocity channels being used

        // DAQ SETTINGS

        voltage_vec ai_min_voltages_;            // vector of minimum input voltages allowed by the board 
        voltage_vec ai_max_voltages_;            // vector of maximum input voltages allowed by the board 

        voltage_vec ao_min_voltages_;            // vector of minimum output voltages allowed by the board 
        voltage_vec ao_max_voltages_;            // vector of maximum output voltages allowed by the board 
        voltage_vec ao_initial_voltages_;        // vector of voltages analog outputs will go to when the program starts 
        voltage_vec ao_final_voltages_;          // vector of voltages analog outputs will go to when the program finishes 
        voltage_vec ao_expire_voltages_;         // vector of voltages analog outputs will go to if the watchdog expires 

        dsignal_vec do_initial_signals_;         // vector of signals digital outputs will go to when the program starts 
        dsignal_vec do_final_signals_;           // vector of signals digital outputs will go to when the program finishes 
        dsignal_vec do_expire_signals_;          // vector of signals digital outputs will go to if the watchdog expires

        uint32_vec  encoder_quadrature_factors_; // vector of encoder quadrature factors, e.g. 1X, 2X, 4X

        // HELPER FUNCTIONS

        // returns index of a channel number in the channels vector
        channel_vec::size_type channel_number_to_index(const channel_vec& channels, const channel channel_number);  
        // turns channel number input such as {3, 1, 1, 2} to {1, 2, 3}  
        channel_vec sort_and_reduce_channels(channel_vec channels);     

    public:

        // CHANNEL STRUCTS AND SETS

        struct Channel { 
            Daq* daq_; 
            channel channel_number_; 
            Channel() : daq_(nullptr), channel_number_(-1) {};
            Channel(Daq* daq, channel channel_number) : daq_(daq), channel_number_(channel_number) {}
        };

        struct Ai : Channel { 
            Ai() : Channel() {}
            Ai(Daq* daq, channel channel_number) : Channel(daq, channel_number) {}
            voltage get_voltage() { return daq_->get_analog_voltage(channel_number_); }            
        };

        struct Ao : Channel {
            Ao() : Channel() {}
            Ao(Daq* daq, channel channel_number) : Channel(daq, channel_number) {}
            void set_voltage(voltage new_voltage) { daq_->set_analog_voltage(channel_number_, new_voltage ); }
        };

        struct Di : Channel {
            Di() : Channel() {}
            Di(Daq* daq, channel channel_number) : Channel(daq, channel_number) {}
            dsignal get_signal() { return daq_->get_digital_signal(channel_number_); }
        };

        struct Do : Channel {
            Do() : Channel() {}
            Do(Daq* daq, channel channel_number) : Channel(daq, channel_number) {}
            void set_signal(dsignal new_signal) { daq_->set_digital_signal(channel_number_, new_signal); }
        };

        struct Encoder : Channel {
            Encoder() : Channel() {}
            Encoder(Daq* daq, channel channel_number) : Channel(daq, channel_number) {}
            int32 get_count() { return daq_->get_encoder_count(channel_number_); }
            uint32 get_quadrature_factor() { return daq_->get_encoder_quadrature_factor(channel_number_); }
        };

        struct EncRate : Channel {
            EncRate() : Channel() {}
            EncRate(Daq* daq, channel channel_number) : Channel(daq, channel_number) {}
            double get_rate() { return daq_->get_encoder_rate(channel_number_); }
            uint32 get_quadrature_factor() { return daq_->get_encoder_quadrature_factor(channel_number_); }            
        };

        // FUNCTIONS FOR GETTING CHANNEL STRUCTS AND SETS
        // these functions are intended to act like indexed vectors 
        virtual Ai ai_(channel channel_number) { return Ai(this, channel_number); }
        virtual Ao ao_(channel channel_number) { return Ao(this, channel_number); }
        virtual Di di_(channel channel_number) { return Di(this, channel_number); }
        virtual Do do_(channel channel_number) { return Do(this, channel_number); }
        virtual Encoder encoder_(channel channel_number) { return Encoder(this, channel_number); }
        virtual EncRate encrate_(channel channel_number) { return EncRate(this, channel_number); }
        
    };


}
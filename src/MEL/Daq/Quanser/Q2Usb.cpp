#include <MEL/Daq/Quanser/Q2Usb.hpp>
#include <MEL/Utility/System.hpp>
#include <MEL/Logging/Log.hpp>
#include <hil.h>

namespace mel {

//==============================================================================
// STATIC VARIABLES
//==============================================================================

 uint32 NEXT_Q2USB_ID = 0;

//==============================================================================
// CLASS DEFINITIONS
//==============================================================================

Q2Usb::Q2Usb(QuanserOptions options, bool auto_open, uint32 id) :
    QuanserDaq("q2_usb", id, options),
    AI(*this),
    AO(*this),
    DIO(*this),
    encoder(*this),
    watchdog(*this, milliseconds(100))
{
    // increment next_id_
    ++NEXT_Q2USB_ID;
    // set channel numners
    AI.set_channel_numbers({ 0,1 });
    AO.set_channel_numbers({ 0,1 });
    DIO.set_channel_numbers({ 0, 1, 2, 3, 4, 5, 6, 7, 8 });
    DIO.set_directions({ In,In,In,In,In,In,In,In,Out });
    // add modules
    add_module(static_cast<AnalogInput*>(&AI));
    add_module(static_cast<AnalogOutput*>(&AO));
    add_module(static_cast<DigitalInputOutput*>(&DIO));
    add_module(static_cast<Encoder*>(&encoder));
    // if open true, open automatically
    if (auto_open)
        open();
}


Q2Usb::~Q2Usb() {

    // if enabled, disable
    if (is_enabled())
        disable();
    // if open, close
    if (open_) {
        // set default options on program end
        set_options(QuanserOptions());
        close();
    }
    // decrement next_id_
    --NEXT_Q2USB_ID;
}

bool Q2Usb::open() {
    // open as QDaq
    if (!QuanserDaq::open())
        return false;
    // clear watchdog (precautionary, ok if fails)
    watchdog.stop();
    // clear the watchdog (precautionary, ok if fails)
    watchdog.clear();
    // set default expire values (digital = LOW, analog = 0.0V)
    if (!AO.set_expire_values(std::vector<Voltage>(2, 0.0))) {
        close();
        return false;
    }
    if (!DIO.set_expire_values(std::vector<Logic>(9, Low))) {
        close();
        return false;
    }
    // allow changes to take effect
    sleep(milliseconds(10));
    return true;
}

bool Q2Usb::close() {
    // stop watchdog (precautionary, ok if fails)
    watchdog.stop();
    // clear the watchdog (precautionary, ok if fails)
    watchdog.clear();
    // allow changes to take effect
    sleep(milliseconds(10));
    // close as QDaq
    return QuanserDaq::close();
}

bool Q2Usb::enable() {
    if (!open_) {
        LOG(Error) << "Unable to call " << __FUNCTION__ << " because "
            << get_name() << " is not open";
        return false;
    }
    // enable each module
    if (!AI.enable())
        return false;
    if (!AO.enable())
        return false;
    if (!DIO.enable())
        return false;
    if (!encoder.enable())
        return false;
    // allow changes to take effect
    sleep(milliseconds(10));
    return Device::enable();
}

bool Q2Usb::disable() {
    if (!open_) {
        LOG(Error) << "Unable to call " << __FUNCTION__ << " because "
            << get_name() << " is not open";
        return false;
    }
    // disable each module
    if (!AI.disable())
        return false;
    if (!AO.disable())
        return false;
    if (!DIO.disable())
        return false;
    if (!encoder.disable())
        return false;
    // stop watchdog (precautionary, ok if fails)
    watchdog.stop();
    // clear the watchdog (precautionary, ok if fails)
    watchdog.clear();
    // allow changes to take effect
    sleep(milliseconds(10));
    return Device::disable();
}

bool Q2Usb::update_input() {
    if (!open_) {
        LOG(Error) << "Unable to call " << __FUNCTION__ << " because "
            << get_name() << " is not open";
        return false;
    }
    if (AI.update() && encoder.update() && DIO.update())
        return true;
    else {
        return false;
    }
}

bool Q2Usb::update_output() {
    if (!open_) {
        LOG(Error) << "Unable to call " << __FUNCTION__ << " because "
            << get_name() << " is not open";
        return false;
    }
    if (AO.update() && DIO.update())
        return true;
    else
        return false;
}

bool Q2Usb::identify(uint32 input_channel_number, uint32 outout_channel_number) {
    if (!open_) {
        LOG(Error) << "Unable to call " << __FUNCTION__ << " because "
            << get_name() << " is not open";
        return false;
    }
    InputOutput<Logic>::Channel di_ch = DIO.get_channel(input_channel_number);
    InputOutput<Logic>::Channel do_ch = DIO.get_channel(outout_channel_number);
    for (int i = 0; i < 5; ++i) {
        do_ch.set_value(High);
        do_ch.update();
        sleep(milliseconds(10));
        di_ch.update();
        if (di_ch.get_value() != High)
            return false;
        do_ch.set_value(Low);
        do_ch.update();
        sleep(milliseconds(10));
        di_ch.update();
        if (di_ch.get_value() != Low)
            return false;
    }
    return true;
}

void Q2Usb::set_led(Logic value) {
    DIO[8].set_value(value);
}

std::size_t Q2Usb::get_q2_usb_count() {
    return QuanserDaq::get_qdaq_count("q2_usb");
}

uint32 Q2Usb::next_id() {
    return NEXT_Q2USB_ID;
}

} // namespace mel


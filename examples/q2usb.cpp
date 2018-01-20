#include <MEL/Daq/Quanser/Q2Usb.hpp>
#include <MEL/Utility/Timer.hpp>
#include <MEL/Math/Waveform.hpp>
#include <MEL/Utility/Windows/Keyboard.hpp>

using namespace mel;

// create global stop variable CTRL-C handler function
static bool stop = false;
static void handler(int var) {
    stop = true;
}

int main() {

    // register CTRL-C handler
    register_ctrl_c_handler(handler);

    // create default Q28 USB object (all channels enabled)
    Q2Usb q2;

    // ask for user input
    prompt("Press ENTER to open and enable Q8 USB.");

    // enable Q8 USB
    q2.enable();

    // ask for user input
    prompt("Connect an encoder to channel 0 then press ENTER to start test.");

    // create 10 Hz Timer
    Timer timer(milliseconds(100));

    // start encoder loop
    while (timer.get_elapsed_time() < seconds(5) && !stop) {
        q2.update_input();
        print(q2.encoder.get_value(0));
        timer.wait();
    }

    stop = false;

    // ask for user input
    prompt("Connect AO0 to AI0 then press ENTER to start test.");

    // create Waveform
    Waveform wave(Waveform::Sin, seconds(0.25), 5.0);

    // start analog loopback loop
    timer.restart();
    while (timer.get_elapsed_time() < seconds(5) && !stop) {
        q2.update_input();
        print(q2.analog_input.get_value(0));
        double voltage = wave.evaluate(timer.get_elapsed_time());
        q2.analog_output.set_value(0, voltage);
        q2.update_output();
        timer.wait();
    }

    stop = false;

    // ask for user input
    prompt("Connect DIO0 to DIO7 then press ENTER to start test.");

    // set directions
    q2.digital_io.set_direction(7, QDigitalInputOutput::Direction::Output);

    logic signal = HIGH;

    // start analog loopback loop
    timer.restart();
    while (timer.get_elapsed_time() < seconds(5) && !stop) {
        q2.digital_io.update();
        print((int)q2.digital_io[0].get_value());
        signal = !signal;
        q2.digital_io[7].set_value(signal);
        q2.digital_io.update();
        timer.wait();
    }

    // ask for user input
    prompt("Press enter then hold L to turn on LED.");

    // make sure LED Mode is set to User
    QOptions options;
    options.set_led_mode(QOptions::LedMode::User);
    q2.set_options(options);

    timer.restart();
    while (timer.get_elapsed_time() < seconds(10) && !stop) {
        if (Keyboard::is_key_pressed(Keyboard::L))
            q2.set_led(HIGH);
        else
            q2.set_led(LOW);
        q2.update_output();
        timer.wait();
    }

    // disable Q8 USB
    q2.disable();

    // close Q8 USB
    q2.close();
}
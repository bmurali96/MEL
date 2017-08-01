#pragma once
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <cmath>
#include <assert.h>
#include <fstream>
#include "qbmove_communications.h"
#include "definitions.h"
#include "commands.h"

#define CUFF_ID 1

class Cuff {

public:

	int enable();
    int disable();
	void set_motor_positions(short int motor_position_0, short int motor_position_1);
	void pretensioning(short int*);

private:

	short int reference_motor_positions_[2];
	short int actual_motor_positions_[2];
	short int actual_motor_currents_[2];

	comm_settings comm_settings_t_;
	boost::mutex io_mutex_;
    boost::thread io_thread_;
    int io_thread_func();

	int open_port();
    int port_selection();

	volatile bool io_loop_;

};
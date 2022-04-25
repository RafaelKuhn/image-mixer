#include <iostream>
#include <ctime>

#include "timer.h"

Timera& Timera::get_instance() {
	static Timer _instance;
	return _instance;
}

void Timera::start_measure() {
	start_time = std::time(NULL);
	std::cout << "-> " << start_time;
}


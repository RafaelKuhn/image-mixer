#include <iostream>
#include <chrono>

#include "timer.h"


Timer& Timer::get_instance() {
	static Timer _instance;
	return _instance;
}

void Timer::start_measure() {
	using std::chrono::duration_cast;
	using std::chrono::system_clock;
	using std::chrono::milliseconds;
	// using std::chrono::seconds;

	start_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	// std::cout << "start time: " << start_time << "\n";
}

void Timer::print_measure(const char* message) {
	// using std::chrono::seconds;
	using std::chrono::milliseconds;
	using std::chrono::duration_cast;
	using std::chrono::system_clock;

	long end_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	long elapsed_time = end_time - start_time;
	// cout << "end time:   " << end_time << "\n";
	std::cout << message << elapsed_time << " ms | " << elapsed_time/1000.0 << " s\n";
}
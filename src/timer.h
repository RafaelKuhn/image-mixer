#pragma once

#include <iomanip> // setprecision
#define RND(dec) std::fixed << std::setprecision(3) << dec

class Timer {
private:
	long start_time {0};

public:
	static Timer& get_instance();

	void start_measure();
	void print_measure(const char* message);
};
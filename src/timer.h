#pragma once

// #include <iostream>
// #include <ctime>

class Timera {
public:
	static Timera& get_instance();

	long start_time {0};

	void start_measure();

};
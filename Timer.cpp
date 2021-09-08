#pragma once
#include <chrono>
#include <iostream>
//#include <string_view>

class Timer {
public:
	Timer() :m_start(std::chrono::steady_clock::now()) {

	};
	int GetResult() {
		auto end = std::chrono::steady_clock::now();
		auto diff = end - m_start;
		//std::cout
		//	<< message
		//	<< " : "
		return (unsigned int)std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
		//<< std::endl;
	}
	~Timer() {};

private:
	std::chrono::steady_clock::time_point m_start;
};

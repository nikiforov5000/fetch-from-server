#include <iostream>
#include <string>
#include <thread>
#include "Vector.h"

int main() {
	auto startTime{ std::chrono::system_clock::now() };
	Vector vec(2018);
	vec.fillVec();
	double vecMedian{ vec.getMedian() };

	// Print 
	auto endTime{ std::chrono::system_clock::now() };
	auto duration{ std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() };
	std::cout << "Median is: " << std::to_string(vecMedian) << std::endl;
	std::cout << "Total sec: " << duration / 1000 << std::endl;
	std::cout << ((duration * 2018) / 1000) / vec.GetSize() << " sec per 2018" << std::endl;
	
	return 0;
}

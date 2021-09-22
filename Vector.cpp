#include <execution>
#include <future>
#include "Vector.h"
#pragma once
#include <execution>
#include <iostream>

bool Vector::isFull(size_t start, size_t end) {
	for (size_t i = start; i < end && i < m_size; ++i) {
		if (m_vec[i].GetValue() == -1) {
			return false;
		}
	}
	return true;
}

size_t Vector::emptyElements(size_t start, size_t end) {
	size_t count{ 0 };
	for (size_t i = start; i < end && i < m_size; ++i) {
		if (m_vec[i].GetValue() == -1) {
			count++;
		}
	}
	return count;
}

void printStopwatch(size_t seconds) {
	while (seconds) {
		std::cout << seconds-- << " ";
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	std::cout << std::endl;
}

void Vector::fillVec() {
	std::vector<std::future<void>> results(m_size);
	float step{ 100 };
	float start{ 1 };
	float end{ start + step };
	float sleepTime{ 35 };
	do {
		for (size_t i = start; i < end && i < m_size; ++i) {
			results[i] = std::async(std::launch::async, [this, i]() { m_vec[i].fillElement(); });
		}
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		if (emptyElements(start, end) > 3) {
			auto temp = std::async(std::launch::async, printStopwatch, sleepTime);
			std::this_thread::sleep_for(std::chrono::seconds((int)sleepTime));
		std::cout << "______________EMPTY is " << emptyElements(start, end) << " out of " << (step) << std::endl;
		}
		size_t numOfEmpty{ emptyElements(1, m_size) };
		if (emptyElements(start, end) > step / 2.0f) {
			step = (step / 1.2f) + 1.0f;
			sleepTime *= 1.2f;
			//std::cout << "=====STEP IS " << step << std::endl;
			//std::cout << "+++++++++++++++++++SLEEP IS "<< sleepTime << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		else if (emptyElements(start, end) < step / 5.0f) {
			step *= 1.2; 
			sleepTime / 1.2f;
			//std::cout << "=====STEP IS " << step << std::endl;
			//std::cout << "+++++++++++++++++++SLEEP IS " << sleepTime << std::endl;
		}
		if (numOfEmpty < step) {
			start = 1;
			end = m_size;
			sleepTime /= 2;
		}
		else {
			start = end;
			end += step;
		}
		if (start >= m_size) {
			start = 1;
			end = step;
		}
	} while (!isFull(1, end));
}

struct lessThanValue {
	inline bool operator() (Element& element1, Element& element2) {
		return (element1.GetValue() < element2.GetValue());
	}
};

double Vector::getMedian() {
	std::sort(std::execution::par_unseq, m_vec.begin() + 1, m_vec.end(), lessThanValue());
	if ((m_vec.size() - 1) % 2 == 0) {
		double floorMiddle{ floor(m_vec.size() - 1) / 2 };
		double avg{ (m_vec[floorMiddle].GetValue() + m_vec[floorMiddle + 1].GetValue()) / 2.0 };
		return avg;
	}
	size_t middle{ (m_vec.size() - 1) / 2 };
	return m_vec[middle].GetValue();
}

size_t Vector::GetSize() {
	return m_vec.size();
}

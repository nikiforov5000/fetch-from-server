#include <execution>
#include "Vector.h"

bool Vector::isFull(size_t start, size_t end) {
	for (size_t i = start; i < end && i < m_size; ++i) {
		if (m_vec[i].GetValue() == -1) {
			return false;
		}
	}
	return true;
}

size_t Vector::emptyElements(double start, double end) {
	double count{ 0 };
	for (double i = start; i < end && i < m_size; ++i) {
		if (m_vec[i].GetValue() == -1) {
			count++;
		}
	}
	return count;
}

void Vector::fillVec() {
	std::vector<std::future<void>> results(m_size);
	double step{ 100 };
	double start{ 1 };
	double end{ start + step };
	double sleepTime{ 35 };
	do {
		for (size_t i = (size_t)start; i < (size_t)end && i < m_size; ++i) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			results[i] = std::async(std::launch::async, [this, i]() { m_vec[i].fillElement(); });
		}
		if (emptyElements(start, end) > 3) {
			std::this_thread::sleep_for(std::chrono::seconds((int)sleepTime));
		}
		size_t numOfEmpty{ emptyElements(1, m_size) };
		if (emptyElements(start, end) > step / 2.0) {
			step = (step / 1.2) + 1.0;
			sleepTime *= 1.2;
		}
		else if (emptyElements(start, end) < step / 5.0) {
			step *= 1.2; 
			sleepTime /= 1.2;
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
		double floorMiddle{ floor(m_vec.size() - 1) / 2.0f };
		double avg{ ((double)m_vec[floorMiddle].GetValue() + (double)m_vec[floorMiddle + 1].GetValue()) / 2.0f };
		return avg;
	}
	size_t middle{ (m_vec.size() - 1) / 2 };
	return m_vec[middle].GetValue();
}

size_t Vector::GetSize() {
	return m_vec.size();
}

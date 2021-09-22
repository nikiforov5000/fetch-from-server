#include <execution>
#include <future>
#include "Vector.h"

bool Vector::isFull(size_t start, size_t end) {
	for (size_t i = start; i < end && i < m_size; ++i) {
		if (m_vec[i].GetValue() == -1) {
			return false;
		}
	}
	return true;
}

size_t Vector::emptyElements(float start, float end) {
	float count{ 0 };
	for (float i = start; i < end && i < m_size; ++i) {
		if (m_vec[i].GetValue() == -1) {
			count++;
		}
	}
	return count;
}

void Vector::fillVec() {
	std::vector<std::future<void>> results(m_size);
	float step{ 100 };
	float start{ 1 };
	float end{ start + step };
	float sleepTime{ 35 };
	do {
		for (size_t i = (size_t)start; i < (size_t)end && i < m_size; ++i) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			results[i] = std::async(std::launch::async, [this, i]() { m_vec[i].fillElement(); });
		}
		if (emptyElements(start, end) > 3) {
			std::this_thread::sleep_for(std::chrono::seconds((int)sleepTime));
		}
		size_t numOfEmpty{ emptyElements(1, m_size) };
		if (emptyElements(start, end) > step / 2.0f) {
			step = (step / 1.2f) + 1.0f;
			sleepTime *= 1.2f;
		}
		else if (emptyElements(start, end) < step / 5.0f) {
			step *= 1.2f; 
			sleepTime / 1.2f;
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
		double avg{ ((double)m_vec[floorMiddle].GetValue() + (double)m_vec[floorMiddle + 1].GetValue()) / 2.0 };
		return avg;
	}
	size_t middle{ (m_vec.size() - 1) / 2 };
	return m_vec[middle].GetValue();
}

size_t Vector::GetSize() {
	return m_vec.size();
}

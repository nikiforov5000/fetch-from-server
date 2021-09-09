#include <execution>
#include "Vector.h"

bool Vector::isFull() {
	for (auto e : m_vec) {
		if (e.GetValue() == "-1") {
			return false;
		}
	}
	return true;
}

void Vector::fillVec() {
	do {
		std::for_each(std::execution::par, m_vec.begin() + 1, m_vec.end(), [this](Element& e) {e.fillElement(); });
	} while (!isFull());
}

struct lessThanValue {
	inline bool operator() (Element& element1, Element& element2) {
		return (std::stoi(element1.GetValue()) < std::stoi(element2.GetValue()));
	}
};

double Vector::getMedian() {
	std::sort(std::execution::par_unseq, m_vec.begin() + 1, m_vec.end(), lessThanValue());
	if ((m_vec.size() - 1) % 2 == 0) {
		double floorMiddle{ floor(m_vec.size() - 1) / 2 };
		double avg{ (std::stod(m_vec[floorMiddle].GetValue()) + std::stod(m_vec[floorMiddle + 1].GetValue())) / 2 };
		return avg;
	}
	size_t middle{ (m_vec.size() - 1) / 2 };
	return std::stod(m_vec[middle].GetValue());
}
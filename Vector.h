#include <vector>
#pragma once
#include "Element.h"

class Vector {
private:
	size_t m_size{};
	std::vector<Element> m_vec{m_size + 1};
public:
	Vector(size_t size) :m_size(size){
		++m_size;
		for (size_t i = 1; i < m_size; ++i) {
			m_vec[i].SetIndex(i);
			//m_vec[i].SetValue(-1);
		}
	};
	bool isFull(size_t start, size_t end);
	size_t emptyElements(double start, double end);
	void fillVec();
	double getMedian();
	size_t GetSize();
	~Vector() {};
};


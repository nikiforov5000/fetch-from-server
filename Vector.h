#pragma once
#include "Element.h"
#include <vector>
class Vector {
private:
	size_t m_size{};
	std::vector<Element> m_vec{m_size + 1};
public:
	Vector(size_t size) :m_size(size){
		++m_size;
		std::string minusOne{ "" };
		for (size_t i = 1; i < m_size; ++i) {
			m_vec[i].SetIndex(i);
			m_vec[i].SetValue(minusOne);
		}
	};
	bool isFull(size_t start, size_t end);
	size_t emptyElements(size_t start, size_t end);
	void fillVec();
	double getMedian();
	size_t GetSize();

	~Vector() {};
};


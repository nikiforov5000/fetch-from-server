#pragma once
#include "Element.h"
#include <vector>
class Vector {
private:
	size_t m_size{};
	std::vector<Element> m_vec{m_size + 1};
public:
	Vector(size_t size) :m_size(size){
		std::string minusOne{ "-1" };
		for (size_t i = 1; i < m_vec.size(); ++i) {

			m_vec[i].SetIndex(i);
			m_vec[i].SetValue(minusOne);
		}
	};
	bool isFull();
	void fillVec();
	double getMedian();

	~Vector() {};
};


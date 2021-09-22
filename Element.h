#pragma once
#include <iostream>
#include <string>

class Element {
private:
	std::string m_index;
	int m_value{ -1 };
public:
	Element() {};
	Element(std::string index);

	void SetValue(int strValue);
	const int GetValue();

	void SetIndex(size_t strIndex);
	std::string GetIndex();

	void writeToFile();
	void fillElement();

	~Element() {};
};

int fetchData(std::string& strInput);

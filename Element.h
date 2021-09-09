#pragma once
#include <string>

class Element {
private:
	std::string m_index;
	std::string m_value;
public:
	Element() {};
	Element(std::string index, std::string value);

	void SetValue(std::string& strValue);
	std::string GetValue();

	void SetIndex(size_t strIndex);
	std::string GetIndex();

	void readFromFile();
	void writeToFile();
	void fillElement();

	~Element() {};
};
std::string fetchData(std::string& strInput);


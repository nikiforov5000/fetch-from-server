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

	void SetIndex(std::string& strIndex);
	std::string GetIndex();

	~Element() {};
};

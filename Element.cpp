#include "Element.h"
#include <fstream>

Element::Element(std::string index, std::string value) :m_index(index), m_value(value) {

};
void Element::SetValue(std::string& strValue) {
	m_value = strValue;
}
std::string Element::GetValue() {
	return m_value;
}
void Element::SetIndex(size_t strIndex) {
	m_index = std::to_string(strIndex);
}
std::string Element::GetIndex() {
	return m_index;
}

inline bool exists(std::string& fileName) {
	std::string pathPrefix{ "./data/" };
	std::ifstream file(pathPrefix.append(fileName).c_str());
	return file.good();
}

void Element::readFromFile() {
	std::string filename{ "./data/" };
	std::ifstream input{ filename.append(m_index) };
	std::string buffer;
	input >> buffer;
	m_value = buffer;
}

void Element::writeToFile() {
	std::string filename{ "./data/" };
	std::ofstream out{ filename.append(m_index) };
	if (m_value == "-1") {
		;
	}
	out << m_value;
}

void Element::fillElement() {
	if (exists(m_index)) {
		readFromFile();
		return;
	}
	m_value = fetchData(m_index);
	writeToFile();
}
#include "Element.h"

#include <fstream>
#include <future>

Element::Element(std::string index) :m_index(index) {

};
void Element::SetValue(int strValue) {
	m_value = strValue;
}
const int Element::GetValue() {
	return m_value;
}
void Element::SetIndex(size_t strIndex) {
	m_index = std::to_string(strIndex);
}
std::string Element::GetIndex() {
	return m_index;
}

int readFromFile(std::string& index) {
	std::string filename{ "./data/" };
	std::ifstream input{ filename.append(index) };
	std::string buffer;
	input >> buffer;
	return std::stoi(buffer);
}

inline bool exists(std::string& fileName) {
	std::string pathPrefix{ "./data/" };
	std::ifstream file(pathPrefix.append(fileName).c_str());
	return file.good() && readFromFile(fileName) != 0;
}

void Element::writeToFile() {
	if (m_value == 0) {
		return;
	}
	std::string filename{ "./data/" };
	std::ofstream out{ filename.append(m_index) };
	out << m_value;
}
void Element::fillElement() {					//wait_for version
	if (exists(m_index)) {
		m_value = readFromFile(m_index);
		return;
	}
	do {
		m_value = fetchData(m_index);
	} while (m_value == 0);
	writeToFile();
}

//void Element::fillElement() {					//for_each version
//	if (exists(m_index)) {
//		m_value = readFromFile(m_index);
//		return;
//	}
//	m_value = fetchData(m_index);
//	writeToFile();
//}
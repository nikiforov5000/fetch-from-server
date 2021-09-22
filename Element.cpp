#include "Element.h"

#include <fstream>
#include <future>

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

std::string readFromFile(std::string& index) {
	std::string filename{ "./data/" };
	std::ifstream input{ filename.append(index) };
	std::string buffer;
	input >> buffer;
	return buffer;
}

inline bool exists(std::string& fileName) {
	std::string pathPrefix{ "./data/" };
	std::ifstream file(pathPrefix.append(fileName).c_str());
	return file.good() && readFromFile(fileName) != "";
}

void Element::writeToFile() {
	if (m_value == "") {
		return;
	}
	std::string filename{ "./data/" };
	std::ofstream out{ filename.append(m_index) };
	out << m_value;
}
void Element::fillElement() {					//wait_for version
	if (exists(m_index)) {
		m_value = readFromFile(m_index);
		std::cout << "Read from file: " << m_value << std::endl;
		return;
	}
	do {
		m_value = fetchData(m_index);
	} while (m_value == "");
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
#include "Element.h"

Element::Element(std::string index, std::string value) :m_index(index), m_value(value) {

};
void Element::SetValue(std::string& strValue) {
	m_value = strValue;
}
std::string Element::GetValue() {
	return m_value;
}
void Element::SetIndex(std::string& strIndex) {
	m_index = strIndex;
}
std::string Element::GetIndex() {
	return m_index;
}


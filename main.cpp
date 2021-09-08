#include <array>
#include <execution>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include "Timer.cpp"
#include <vector>
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

struct Element {
	std::string m_index{};
	std::string m_value{};
};

void writeToFile(Element& e) {
	std::string filename{ "./data/" };
	std::ofstream out{ filename.append(e.m_index) };
	out << e.m_value;
}

void getNumber(Element& e, const std::string& str) {
	const std::string validSet{ "0123456789" };
	auto substrStart{ str.find_first_of(validSet) };
	if (substrStart == str.npos) {
		return;
	}
	auto substrEnd{ str.find_first_of(validSet, substrStart) };
	if (substrEnd == str.npos) {
		substrEnd = str.length();
	}
	e.m_value = std::stoi(str.substr(substrStart, substrStart + substrEnd));
}

inline bool exists(Element& element) {
	std::string pathPrefix{ "./data/" };
	std::ifstream file(pathPrefix.append(element.m_index).c_str());
	return file.good();
}

std::string fetchData(std::string& strInput) {
	using namespace std::chrono_literals;
	std::string response{};
	// Initialize WinSock
	WSADATA WSAData{};
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &WSAData);
	if (wsResult != 0) {
		std::cerr << "[ERROR] Can't start winsock, Err# " << wsResult << std::endl;
		return fetchData(strInput);
	}
	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cerr << "[ERROR] Can't create socket, Err# " << WSAGetLastError() << std::endl;
		return fetchData(strInput);
	}
	// Fill in a hint structure
	std::string ipAddress{ "88.212.241.115" };
	int port = 2013;						
	sockaddr_in hint{};
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		std::cerr << "[ERROR] Can't connect to server. Err# " << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return fetchData(strInput);
	}
	// Do-while loop to send and receive data
	char buff;
	std::string userInput{ strInput };
	userInput.append("\n");
	//Send number to server
	std::cout << "sent:" << userInput << std::endl;//////////////////////////////////////////
	int sendResult{ send(sock, userInput.c_str(), userInput.size() + 1, 0) };
	if (sendResult != SOCKET_ERROR) {
		//Wait for response
		size_t count{ 0 };
		do {
			ZeroMemory(&buff, sizeof(buff));
			recv(sock, &buff, sizeof(buff), MSG_PARTIAL);
			std::cout << "buff is " << buff << std::endl;
			if (WSAGetLastError() != 0) {
				std::cout << "[ERROR]" << WSAGetLastError() << std::endl;
				if (WSAGetLastError() == 10053 || WSAGetLastError() == 10054) {
					std::this_thread::sleep_for(10s);
					fetchData(strInput);
				}
			}
			if (buff >= 48 && buff <= 57) {
				response.append(std::string(1, buff));
			}
			if (count++ > 100) {
				return fetchData(strInput);
			}
		} while (buff != '\n');
		if (response.length() > 0) {
			//Echo response to console
		}
		else {
			return fetchData(strInput);
		}
	}
	// Close
	closesocket(sock);
	WSACleanup();
	return response;
}


void readFromFile(Element& element) {
	std::string filename{ "./data/" };
	std::ifstream input{ filename.append(element.m_index) };
	input >> element.m_value;
}
void fillElement(Element& element){
	if (exists(element)) {
		std::cout << "########################################" << std::endl;
		readFromFile(element);
		return;
	}
	element.m_value = fetchData(element.m_index);
	writeToFile(element);
}

void fillVec(std::vector<Element>& vec) {
	std::for_each(std::execution::par, vec.begin() + 1, vec.end(), fillElement);
}

struct lessThanValue {
	inline bool operator() (const Element& element1, const Element& element2) {
		return (std::stoi(element1.m_value) < std::stoi(element2.m_value));
	}
};

double getMedian(std::vector<Element>& vec) {
	std::sort(std::execution::par_unseq, vec.begin() + 1, vec.end(), lessThanValue());
	if ((vec.size() - 1) % 2 == 0) {
		double floorMiddle{ floor(vec.size() - 1) / 2 };
		double avg{ (std::stod(vec[floorMiddle].m_value) + std::stod(vec[floorMiddle + 1].m_value)) / 2 };
		return avg;
	}
	size_t middle{ (vec.size() - 1) / 2 };
	return std::stod(vec[middle].m_value);
}

std::unique_ptr<std::vector<Element>> vec(new std::vector<Element>); 

int main() {
	// Prepare a vector
	std::vector<Element> vec(2019);
	for (size_t i = 1; i < vec.size(); ++i) {
		vec[i].m_index = std::to_string(i);
		vec[i].m_value = "-1";
	}
	
	fillVec(vec);
	double result{ getMedian(vec) };
	
	//
	//std::string checkResult{ "Check " };
	//std::cout << fetchData(checkResult.append(std::to_string(result)));
	std::cout << result;
	
	/*Element testElement;
	testElement.m_index = "2" ;
	testElement.m_value = "-1";
	fillElement(testElement);
	std::cout << testElement.m_value;*/

	std::cout << "------------------------------------------------" << std::endl;

	return 0;
}


#include <fstream>
#include <future>
#include "Element.h"
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")


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


int Element::fetchData() {
	using namespace std::chrono_literals;
	int response{ 0 };
	// Initialize WinSock
	WSADATA WSAData{};
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &WSAData);
	if (wsResult != 0) {
		std::cerr << "[ERROR] Can't start winsock, Err# " << wsResult << std::endl;
		return -1;
	}
	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cerr << "[ERROR] Can't create socket, Err# " << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return -1;
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
		return -1;
	}
	char buff;
	std::string userInput{ m_index };
	userInput.append("\n");
	//Send number to server
	int sendResult{ send(sock, userInput.c_str(), (int)userInput.size() + 1, 0) };
	// Do-while loop to send and receive data
	if (sendResult != SOCKET_ERROR) {
		//Wait for response
		size_t count{ 0 };
		do {
			ZeroMemory(&buff, sizeof(buff));
			recv(sock, &buff, sizeof(buff), MSG_PARTIAL);
			if (WSAGetLastError() != 0) {
				std::cerr << "[ERROR]" << WSAGetLastError() << std::endl;
				closesocket(sock);
				WSACleanup();
				return -1;
			}
			if (count++ > 96) {
				closesocket(sock);
				WSACleanup();
				return -1;
			}
			if (buff >= 48 && buff <= 57) {
				response = (response * 10) + (buff + 0);
			}
		} while (buff != '\n');
		if (response == -1) {
			closesocket(sock);
			WSACleanup();
			return -1;
		}
	}
	closesocket(sock);
	WSACleanup();
	return response;
}

void Element::fillElement() {
	if (exists(m_index)) {
		m_value = readFromFile(m_index);
		return;
	}
	do {
		m_value = fetchData();
	} while (m_value == -1);
	writeToFile();
}

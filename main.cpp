#include <execution>
#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "Vector.h"
#pragma comment(lib, "ws2_32.lib")

int fetchData(std::string& strInput) {
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
	std::string userInput{ strInput };
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

int main() {
	auto startTime{ std::chrono::system_clock::now() };
	Vector vec(2018);
	vec.fillVec();
	double vecMedian{ vec.getMedian() };

	// Print 
	auto endTime{ std::chrono::system_clock::now() };
	auto duration{ std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() };
	std::cout << "Median is: " << std::to_string(vecMedian) << std::endl;
	std::cout << "Total sec: " << duration / 1000 << std::endl;
	std::cout << ((duration * 2018) / 1000) / vec.GetSize() << " sec per 2018" << std::endl;
	
	return 0;
}

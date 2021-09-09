#include <execution>
#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "Vector.h"
#pragma comment(lib, "ws2_32.lib")

std::string fetchData(std::string& strInput) {
	std::cout << "Fetch Started " << strInput << std::endl;
	using namespace std::chrono_literals;
	std::string response{ "" };
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
	int sendResult{ send(sock, userInput.c_str(), userInput.size() + 1, 0) };
	if (sendResult != SOCKET_ERROR) {
		//Wait for response
		size_t count{ 0 };
		do {
			ZeroMemory(&buff, sizeof(buff));
			recv(sock, &buff, sizeof(buff), MSG_PARTIAL);
			if (WSAGetLastError() != 0) {
				std::cerr << "[ERROR]" << WSAGetLastError() << std::endl;
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
		if (response.length() == 0 || response == "-1") {
			closesocket(sock);
			WSACleanup();
			return fetchData(strInput);
		}
	}
	// Close
	closesocket(sock);
	WSACleanup();
	return response;
}


int main() {
	Vector vec(2018);
	vec.fillVec();
	double vecMedian{ vec.getMedian() };

	return 0;

}


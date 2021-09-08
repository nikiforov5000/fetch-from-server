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

static int RUNNING_THREADS = 0;

struct Element {
	int m_index{};
	std::string m_value{};
};

void Write(Element& e) {
	std::string filename{ "./data/" };
	std::ofstream out{ filename.append(std::to_string(e.m_index)) };
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
void Print(Element e) {
	std::cout << e.m_index << ":" << e.m_value << std::endl;
}

int PrintBuff(char* buff) {
		std::cout << "buff is";
	for (size_t i = 0; i != '\0'; ++i) {
		std::cout << buff[i];
	}
		std::cout << std::endl;
		return 0;
}

inline bool exists(const std::string& name) {
	std::string pathPrefix{ "./data/" };
	std::ifstream file(pathPrefix.append(name).c_str());
	return file.good();
}



void fetchData(Element& element) {
	using namespace std::chrono_literals;
	// Initialize WinSock
	WSADATA WSAData{};
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &WSAData);
	if (wsResult != 0) {
		std::cerr << "[ERROR] Can't start winsock, Err# " << wsResult << std::endl;
		return;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cerr << "[ERROR] Can't create socket, Err# " << WSAGetLastError() << std::endl;
		return;
	}

	// Fill in a hint structure
	std::string ipAddress{ "88.212.241.115" };		// IP Address of the server
	int port = 2013;								// Listening port # on the server
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
		fetchData(element);
		return;
	}

	// Do-while loop to send and receive data
	char buff;
	std::string userInput{ std::to_string(element.m_index).append("\n")};
	std::string response{};

	//Send number to server
	std::cout << "sent:" << userInput << std::endl;//////////////////////////////////////////
	int sendResult{ send(sock, userInput.c_str(), userInput.size() + 1, 0) };
	if (sendResult != SOCKET_ERROR) {
		//Wait for response
		size_t count{ 0 };
		do {
			ZeroMemory(&buff, sizeof(buff));
			recv(sock, &buff, sizeof(buff), MSG_PARTIAL);
			if (WSAGetLastError() != 0) {
				std::cout << "[ERROR]" << WSAGetLastError() << std::endl;
				if (WSAGetLastError() == 10053 || WSAGetLastError() == 10054) {
					std::this_thread::sleep_for(5s);
				}
			}
			if (buff >= 48 && buff <= 57) {
				response.append(std::string(1, buff));
			}
			else if (buff == '\n') {
				break;
			}
			if (count++ > 100) {
				fetchData(element);
				return;
			}
		} while (1);
		if (response.length() > 0) {
			//Echo response to console
			element.m_value = response;
			Print(element);
			Write(element);
		}
		else {
			fetchData(element);
		}
	}
	// Close
	closesocket(sock);
	WSACleanup();
	--RUNNING_THREADS;
	return;
}

std::string readFromFile(Element& element) {
	std::string filename{ "./data/" };
	std::ifstream input{ filename.append(std::to_string(element.m_index)) };
	std::string fileValue;
	input >> fileValue;
	return fileValue;
}
void fillArray(Element& element) {
	++RUNNING_THREADS;
	std::cout << "~~~~~~~~~~~~RUNNING_THREADS___" << RUNNING_THREADS << std::endl;
	if (exists(std::to_string(element.m_index))) {
		std::cout << "########################################" << std::endl;
		element.m_value = readFromFile(element);
		--RUNNING_THREADS;
		return;
	}
	fetchData(element);
}

struct lessThanValue {
	inline bool operator() (const Element& element1, const Element& element2) {
		return (std::stoi(element1.m_value) < std::stoi(element2.m_value));
	}
};

//template<std::size_t SIZE>
double getMedian(std::array<Element, 2019>& arr) {
	std::cout << arr[1009].m_value << "_arr[1009]_" << arr[1010].m_value << "_arr[1010]_" << std::endl;
	std::sort(std::execution::par_unseq, arr.begin() + 1, arr.end(), lessThanValue());
	std::cout << arr[1009].m_value << "_arr[1009]_" << arr[1010].m_value << "_arr[1010]_" << std::endl;
	std::cout << (std::stod(arr[1009].m_value) + std::stod(arr[1010].m_value)) / 2 << std::endl;
	if ((arr.size() - 1) % 2 == 0) {
		double floorMiddle{ floor(arr.size() - 1) / 2 };
		double avg{ (std::stod(arr[floorMiddle].m_value) + std::stod(arr[floorMiddle + 1].m_value)) / 2 };
		return avg;
	}
	size_t middle{ (arr.size() - 1) / 2 };
	return std::stod(arr[middle].m_value);
}

int main() {
	// Prepare an array
	std::array<Element, 2019> arr;
	for (size_t i = 1; i < arr.size(); ++i) {
		//arr[i] = new Element;
		arr[i].m_index = i;
		arr[i].m_value = -1;
	}

	//std::thread_pool
	std::for_each(std::execution::par, arr.begin() + 1, arr.end(), fillArray);
	//double result{ getMedian(arr) };
	std::cout << std::to_string(getMedian(arr));
	//fillArray(arr[2]);
	/*for (size_t i = 1; i < arr.size(); ++i) {
		std::thread thSender1(fetchData, std::ref(arr[i]));
		std::thread thSender2(fetchData, std::ref(arr[i + 1]));
		std::thread thSender3(fetchData, std::ref(arr[i + 2]));
		thSender1.join();
		thSender2.join();
		thSender3.join();
	}*/
	//fetchData(arr[6]);
	//Write();
	//Read();
	std::cout << "------------------------------------------------" << std::endl;

	return 0;
}


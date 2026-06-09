#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <thread>

#pragma comment (lib, "ws2_32.lib")

bool initializeWinsockLib(){
    WSADATA data;

    return WSAStartup(MAKEWORD(2,2), &data) == 0;
}

int main(){

    if (!initializeWinsockLib()){
    std::cout << "Winsock lib failed to initialize" << std::endl;
}

SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

if (serverSocket == INVALID_SOCKET) {
    std::cout << "Socket creation failed" << std::endl;
}

int port = 12345;
std::string serverIPAddress = "127.0.0.1";
sockaddr_in serverAddress;
serverAddress.sin_family = AF_INET;
serverAddress.sin_port = htons(port);
inet_pton(AF_INET, serverIPAddress.c_str(), &(serverAddress.sin_addr));

    if (connect(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR){
        std::cout << "Not able to connect to server" << std::endl;
        closesocket(serverSocket);
        WSACleanup();

        return 1;
    }

    std::cout << "Successfully connected to server" << std::endl;

    std::string message = "Hello there!";
    int bytesSent = send(serverSocket, message.c_str(), message.length(), 0);

    if (bytesSent == SOCKET_ERROR){
        std::cout << "Message sent failed" << std::endl; 
    }

    closesocket(serverSocket);

    WSACleanup();
    return 0;
}
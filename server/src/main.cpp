#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    // Inicializar Winsock
    WSADATA wsData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsData);

    if (result != 0)
    {
        std::cout << "WSAStartup failed!" << std::endl;
        return 1;
    }

    // Crear socket
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listenSocket == INVALID_SOCKET)
    {
        std::cout << "Socket creation failed!" << std::endl;
        WSACleanup();
        return 1;
    }

    // Configurar direccion
    int port = 12345;
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind
    if (bind(listenSocket,
             reinterpret_cast<sockaddr*>(&serverAddr),
             sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cout << "Bind failed!" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Escuchar conexiones
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cout << "Listen failed!" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Waiting for client..." << std::endl;

    // Aceptar cliente
    SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);

    if (clientSocket == INVALID_SOCKET)
    {
        std::cout << "Accept failed!" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Client connected!" << std::endl;

    char buffer[4096];

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));

        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived <= 0)
        {
            std::cout << "Client disconnected!" << std::endl;
            break;
        }

        std::string clientMessage(buffer, bytesReceived);

        std::cout << "\nClient: " << clientMessage << std::endl;

        std::cout << "Server: ";

        std::string serverMessage;
        std::getline(std::cin, serverMessage);

        int bytesSent = send(
            clientSocket,
            serverMessage.c_str(),
            static_cast<int>(serverMessage.size()),
            0);

        if (bytesSent == SOCKET_ERROR)
        {
            std::cout << "Send failed!" << std::endl;
            break;
        }
    }

    closesocket(clientSocket);
    closesocket(listenSocket);
    WSACleanup();

    return 0;
}
#include "../include/Client.h"
#include "./Logger.cpp"
#include <tchar.h>
#include <iostream>
#include <algorithm>

// Constructor and destructor

Client::Client(int port, std::string ipAddress)
    : m_serverPort(port),
    m_serverIPAddress(ipAddress),
      m_serverSocket(INVALID_SOCKET)
{
}

Client::~Client()
{
    if (m_serverSocket != INVALID_SOCKET)
    {
        closesocket(m_serverSocket);
    }

    WSACleanup();
}


// Public functions

bool Client::start() {
    if (!initializeWinsock())
        return false;

    if (!createServerSocket())
        return false;

    if (!bindAndConnect())
        return false;

    Logger::info("Client listening on server port " + std::to_string(m_serverPort));
    return true;
}

void Client::run(){

    std::thread receiver(
        &Client::handleServer,
        this,
        m_serverSocket
    );


    while(true)
    {

        std::string message;

        std::cout << "Message: ";

        getline(
            std::cin,
            message
        );


        send(
            m_serverSocket,
            message.c_str(),
            message.size(),
            0
        );

        std::cout << "Me: " << message << std::endl; 
    }


    receiver.join();
    
}



// Private functions

bool Client::initializeWinsock()
{
    WSADATA wsData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsData);

    if (result != 0)
    {
        Logger::error("WSAStartup failed!");
        return false;
    }

    return true;
}

SOCKET Client::createServerSocket()
{
    m_serverSocket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP);

    if (m_serverSocket == INVALID_SOCKET)
    {
        Logger::error("Socket creation failed!");
        return false;
    }

    return true;
}

bool Client::bindAndConnect()
{
    sockaddr_in serverAddress{};


    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(m_serverPort);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    inet_pton(AF_INET, m_serverIPAddress.c_str(), &(serverAddress.sin_addr));

    
    if (connect(m_serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR){
        Logger::error("Not able to connect to server");
        closesocket(m_serverSocket);
        WSACleanup();

        return 0;
    }

    Logger::info("Succesfully connected to server");
    
    return true;
}

SOCKET Client::handleServer(SOCKET clientSocket)
{
   char buffer[4096];


    while(true)
    {

        int bytesReceived =
            recv(
                clientSocket,
                buffer,
                sizeof(buffer),
                0
            );


        if(bytesReceived <= 0)
        {
            break;
        }


        std::string message(
            buffer,
            bytesReceived
        );


        std::cout
            << "\nOther: "
            << message
            << "\nMessage: ";
    }


    return clientSocket;
}

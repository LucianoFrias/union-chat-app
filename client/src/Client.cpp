#include "../include/Client.h"
#include "./Logger.cpp"
#include <iostream>
#include <algorithm>

using namespace Union::Logging;

// Constructor and destructor

namespace Union::Client {


Client::Client(int port, std::string ipAddress)
    : m_serverPort(port),
    m_serverIPAddress(ipAddress),
      m_clientSocket(INVALID_SOCKET),
      m_connected(false)
{
}

Client::~Client()
{
    if (m_clientSocket != INVALID_SOCKET)
    {
        disconnectFromServer();
        closesocket(m_clientSocket);

    }

    WSACleanup();
}


// Public functions

bool Client::start() {
    if (!initializeWinsock())
        return false;

    if (!createClientSocket())
        return false;

    if (!connectToServer())
        return false;

    Logger::info("Client connecting to server port " + std::to_string(m_serverPort));
    return true;
}

void Client::run(){

    std::thread receiver(
        &Client::receiveMessages,
        this
    );

    std::thread sender(
        &Client::sendMessages,
        this
    );


    sender.join();
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

SOCKET Client::createClientSocket()
{
    m_clientSocket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP);

    if (m_clientSocket == INVALID_SOCKET)
    {
        Logger::error("Socket creation failed!");
        return false;
    }

    return true;
}

bool Client::connectToServer()
{
    sockaddr_in serverAddress{};


    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(m_serverPort);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    inet_pton(AF_INET, m_serverIPAddress.c_str(), &(serverAddress.sin_addr));

    
    if (connect(m_clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR){
        Logger::error("Not able to connect to server");
        closesocket(m_clientSocket);
        WSACleanup();

        return 0;
    }

    Logger::info("Succesfully connected to server");
    m_connected = true;
    
    return true;
}

void Client::disconnectFromServer() {

    if (m_connected.exchange(false))
    {
        shutdown(m_clientSocket, SD_BOTH);
        Logger::info("Disconnected from server!");
        m_connected = false;
    }
}

void Client::receiveMessages()
{
   char buffer[4096];

    while(m_connected)
    {

        int bytesReceived =
            recv(
                m_clientSocket,
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
            << message << "\n";
    }
}


void Client::sendMessages() 
{
    std::string message;

    while(m_connected && std::getline(std::cin, message))
    {
        if (message == "/quit") {
            disconnectFromServer();
            break;
        }

        send(
            m_clientSocket,
            message.c_str(),
            static_cast<int>(message.size()),
            0
        );

        std::cout << "Me: " << message << std::endl; 
    }
}

}


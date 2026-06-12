#include "../include/Client.h"
#include <tchar.h>
#include <iostream>

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

    std::cout << "Client listening on server port "
              << m_serverPort
              << '\n';

    return true;
}

void Client::run(){

    while (true){
    std::string message;
    std::cout << "My message: ";
    std::getline(std::cin, message);

    int bytesSent = send(m_serverSocket, message.c_str(), static_cast<int>(message.size()), 0);

    if (bytesSent == SOCKET_ERROR){
        std::cout << "Message sent failed" << std::endl; 
        return;
    }

        std::string myMessage = message;

        std::cout << "Me: " << myMessage << std::endl;

    }
    
}



// Private functions

bool Client::initializeWinsock()
{
    WSADATA wsData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsData);

    if (result != 0)
    {
        std::cerr << "WSAStartup failed!\n";
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
        std::cerr << "Socket creation failed!\n";
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
        std::cout << "Not able to connect to server" << std::endl;
        closesocket(m_serverSocket);
        WSACleanup();

        return 0;
    }

    std::cout << "Successfully connected to server" << std::endl;


    return true;
}

SOCKET Client::handleServer(SOCKET clientSocket)
{
    char buffer[4096];

    while (true)
    {
        int bytesReceived =
            recv(clientSocket,
                 buffer,
                 sizeof(buffer),
                 0);

        if (bytesReceived <= 0)
        {
            break;
        }

        std::string clientMessage(
            buffer,
            bytesReceived);

        std::cout
            << "\nClient: "
            << clientMessage
            << '\n';

        std::cout << "Clien: ";

        std::string reply;
        std::getline(std::cin, reply);

        int bytesSent =
            send(clientSocket,
                 reply.c_str(),
                 static_cast<int>(reply.size()),
                 0);

        if (bytesSent == SOCKET_ERROR)
        {
            break;
        }
    }
}


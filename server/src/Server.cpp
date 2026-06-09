#include "../include/Server.h"
#include <iostream>

// Constructor and destructor

Server::Server(int port)
    : m_port(port),
      m_listenSocket(INVALID_SOCKET)
{
}

Server::~Server()
{
    if (m_listenSocket != INVALID_SOCKET)
    {
        closesocket(m_listenSocket);
    }

    WSACleanup();
}


// Public functions

bool Server::start() {
    if (!initializeWinsock())
        return false;

    if (!createListeningSocket())
        return false;

    if (!bindAndListen())
        return false;

    std::cout << "Server listening on port "
              << m_port
              << '\n';

    return true;
}

void Server::run(){
    while (true)
    {
        std::cout << "Waiting for client...\n";

        SOCKET clientSocket =
            accept(m_listenSocket,
                   nullptr,
                   nullptr);

        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "Accept failed!\n";
            continue;
        }

        std::cout << "Client connected!\n";

        //HandleClient(clientSocket);

        closesocket(clientSocket);

        std::cout << "Client disconnected!\n";
    }
}



// Private functions

bool Server::initializeWinsock()
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

SOCKET Server::createListeningSocket()
{
    m_listenSocket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP);

    if (m_listenSocket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed!\n";
        return false;
    }

    return true;
}

bool Server::bindAndListen()
{
    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(
            m_listenSocket,
            reinterpret_cast<sockaddr*>(&addr),
            sizeof(addr)) == SOCKET_ERROR)
    {
        std::cerr << "Bind failed!\n";
        return false;
    }

    if (listen(m_listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed!\n";
        return false;
    }

    return true;
}

SOCKET handleClient(SOCKET clientSocket)
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

        std::cout << "Server: ";

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


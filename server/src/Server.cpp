#include "../include/Server.h"
#include "./Logger.cpp"
#include <iostream>
#include <algorithm>

// Constructor and destructor

using namespace Union::Logging;

namespace Union::Server {
    

Server::Server(int port)
    : m_port(port),
      m_listenSocket(INVALID_SOCKET)
{
}

Server::~Server()
{
    stop();
}


// Public functions

bool Server::start() {

    if (!isPortValid(m_port)){
        return false;
    }

    if (!initializeWinsock())
        return false;

    if (!createListeningSocket())
        return false;

    if (!bindAndListen())
        return false;

    
    Logger::info("Server listening on port " + std::to_string(m_port));

    return true;
}

void Server::run(){

    running = true;

    while (running)
    {
        showRemainingNumberOfClients();

        SOCKET clientSocket =
            accept(m_listenSocket,
                   nullptr,
                   nullptr);

        if (clientSocket == INVALID_SOCKET)
        {
            Logger::error("Accept failed!");
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(clientsMutex);

            clients.push_back(clientSocket);
        }
        

        Logger::info("Client connected!");

        std::thread clientThread(
        &Server::handleClient,
        this,
        clientSocket
    );

        clientThread.detach();
    }
}

void Server::stop(){
    running = false;
    Logger::info("Shutting down server...");


    std::lock_guard<std::mutex> lock(clientsMutex);


    for(auto client : clients)
    {
        closesocket(client);
    }


    clients.clear();


    if(m_listenSocket != INVALID_SOCKET)
    {
        closesocket(m_listenSocket);
        m_listenSocket = INVALID_SOCKET;
    }


    WSACleanup();
}


// Private functions

bool Server::initializeWinsock()
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

bool Server::createListeningSocket()
{
    m_listenSocket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP);

    if (m_listenSocket == INVALID_SOCKET)
    {
        Logger::error("Socket creation failed");
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

    if (bind(m_listenSocket, reinterpret_cast<sockaddr*>(&addr), 
    sizeof(addr)) == SOCKET_ERROR)
    {
        Logger::error("Bind failed. Error: " + std::to_string(WSAGetLastError()));
        return false;
    }

    if (listen(m_listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        Logger::error("Listen failed!");
        return false;
    }

    return true;
}

bool Server::isPortValid(int& port){

    if (port <= 0 || port > 65535){
        Logger::error("Port out of bounds. The port should be between 0 and 65535");
        return false;
    }

    return true;

}


void Server::handleClient(SOCKET clientSocket)
{
    char buffer[4096];

    while (running)
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

            broadcast(
            clientSocket,
            clientMessage
        );
    }

    removeClient(clientSocket);
    closesocket(clientSocket);

    Logger::info("Client disconnected!");
    showRemainingNumberOfClients();
}

void Server::broadcast(SOCKET sender, const std::string& message){

    std::lock_guard<std::mutex> lock(clientsMutex);

    for(auto client : clients)
    {
        if(client != sender)
        {
            int result = send(
                client,
                message.c_str(),
                message.size(),
                0
            );  

            if (result == SOCKET_ERROR){
                Logger::error("Broadcast send failed!");
                return;
            }

        }
    }
}

void Server::showRemainingNumberOfClients(){
    std::lock_guard<std::mutex> lock(clientsMutex);
    
    Logger::info("Number of clients connected: " + std::to_string(clients.size()));
}

void Server::removeClient(SOCKET clientSocket){ 
    std::lock_guard<std::mutex> lock(clientsMutex);

    clients.erase(
        std::remove(
            clients.begin(),
            clients.end(),
            clientSocket
        ),
        clients.end()
    );
}


// Getters and Setters

std::vector<SOCKET>& Server::getClients() {
    return clients;
}

}

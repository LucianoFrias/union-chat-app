#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <mutex>
#include <vector>


namespace Union::Server { 


class Server
{
public:
    Server(int port);
    ~Server();

    bool start();
    void run();

private:
    bool initializeWinsock();
    bool createListeningSocket();
    bool bindAndListen();
    void broadcast(SOCKET sender, const std::string& message);
    void handleClient(SOCKET clientSocket);
    void showRemainingNumberOfClients();
    void removeClient(SOCKET clientSocket);
    void shutdownServer();


private:
    std::vector<SOCKET> clients;
    std::mutex clientsMutex;
    int m_port;
    SOCKET m_listenSocket;
};

}

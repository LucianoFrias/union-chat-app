#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>


namespace Union::Server { 


class Server
{
public:
    Server(int port);
    ~Server();

    bool start();
    void run();
    void stop();


public:
    bool initializeWinsock();
    bool createListeningSocket();
    bool bindAndListen();
    bool isPortValid(int& port);

    void broadcast(SOCKET sender, const std::string& message);
    void handleClient(SOCKET clientSocket);
    void showRemainingNumberOfClients();
    void removeClient(SOCKET clientSocket);

public:
    std::vector<SOCKET>& getClients();


private:
    std::vector<SOCKET> clients;
    std::mutex clientsMutex;
    int m_port;
    SOCKET m_listenSocket;

    std::atomic<bool> running{false};
};

}

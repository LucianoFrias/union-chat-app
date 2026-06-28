#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <atomic>

namespace Union::Client {


class Client
{
public:
    Client(int port, std::string ipAddress);
    ~Client();

    bool start();
    void run();

private:
    bool initializeWinsock();
    SOCKET createServerSocket();
    bool bindAndConnect();
    void receiveMessages();
    void handleServer(SOCKET serverSocket);


private:
    int m_serverPort;
    std::string m_serverIPAddress;
    SOCKET m_serverSocket;

    std::atomic<bool> connected;
};

}

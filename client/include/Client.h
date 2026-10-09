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
    SOCKET createClientSocket();
    bool connectToServer();
    void disconnectFromServer();

    void receiveMessages();
    void sendMessages();


private:
    int m_serverPort;
    std::string m_serverIPAddress;
    SOCKET m_clientSocket;

    std::atomic<bool> m_connected;
};

}

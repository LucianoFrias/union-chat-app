#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

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
    SOCKET handleServer(SOCKET serverSocket);


private:
    int m_serverPort;
    std::string m_serverIPAddress;
    SOCKET m_serverSocket;
};
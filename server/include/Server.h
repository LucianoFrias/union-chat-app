#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

class Server
{
public:
    Server(int port);
    ~Server();

    bool start();
    void run();

private:
    bool initializeWinsock();
    SOCKET createListeningSocket();
    bool bindAndListen();
    SOCKET handleClient(SOCKET clientSocket);


private:
    int m_port;
    SOCKET m_listenSocket;
};
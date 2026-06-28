#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>

#include "../include/Client.h"

#pragma comment (lib, "ws2_32.lib")

using namespace Union::Client;


int main(){
    Client client(12345, "127.0.0.1");

     if (!client.start())
    {
        return 1;
    }

    client.run();

    return 0;
}
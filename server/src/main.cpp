#include "../include/Server.h"

int main(){

    Server server(12345);

     if (!server.start())
    {
        return 1;
    }

    server.run();

    return 0;
}
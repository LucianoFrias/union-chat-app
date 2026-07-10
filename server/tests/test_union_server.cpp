#include <gtest/gtest.h>
#include "../include/Server.h"

using namespace Union::Server;

TEST(UnionServerTest, StartServerReturnsTrue)
{
    Server server(8080);

    EXPECT_TRUE(server.start());
}


TEST(UnionServerTest, StartAndStopServer)
{
    Server server(5000);

    ASSERT_TRUE(server.start());

    std::thread t([&]()
    {
        server.run();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    server.stop();

    t.join();
}

TEST(UnionServerTest, CanRestartServer)
{
    {
        Server server(5001);

        ASSERT_TRUE(server.start());
        server.stop();
    }

    {
        Server server(5001);

        ASSERT_TRUE(server.start());
        server.stop();
    }
}

TEST(UnionServerTest, TwoServersSamePort)
{
    Server server1(6000);
    Server server2(6000);

    EXPECT_TRUE(server1.start());

    EXPECT_FALSE(server2.start());

    server1.stop();
}

TEST(UnionServerTest, InvalidNegativePort)
{
    Server server(-1);

    EXPECT_FALSE(server.start());
}

TEST(UnionServerTest, InvalidZeroPort)
{
    Server server(0);

    EXPECT_FALSE(server.start());
}

TEST(UnionServerTest, RemoveExistingClient)
{
    Server server(8080);

    server.getClients().push_back((SOCKET)1);
    server.getClients().push_back((SOCKET)2);
    server.getClients().push_back((SOCKET)3);

    server.removeClient((SOCKET)2);

    EXPECT_EQ(server.getClients().size(), 2);

    EXPECT_EQ(server.getClients()[0], (SOCKET)1);
    EXPECT_EQ(server.getClients()[1], (SOCKET)3);
}

// Usar mocks para testear handleClient() y broadcast()
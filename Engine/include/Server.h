#pragma once
#include <winsock2.h>
#include <iostream>
#include <thread>

class NetworkServer {
public:
    NetworkServer();
    ~NetworkServer();

    void startServer(int port);
    void sendMove(const std::string& move);
    std::string receiveMove();

private:
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize;
    void handleClient();
    void acceptClient();
};

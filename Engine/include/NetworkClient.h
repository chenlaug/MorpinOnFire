#pragma once
#include <winsock2.h>
#include <iostream>
#include <thread>


class NetworkClient {
public:
    NetworkClient();
    ~NetworkClient();

    void connectToServer(const std::string& serverIp, int port);
    void sendMove(const std::string& move);
    std::string receiveMove();

private:
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    void receiveLoop();
};

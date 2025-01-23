#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <winsock2.h>
#include <iostream>
#include <functional>

class NetworkClient {
public:
    NetworkClient();
    ~NetworkClient();

    void connectToServer(const std::string& serverIP, int port);
    void receiveGameStartNotification(std::function<void()> gameStartCallback);

private:
    SOCKET clientSocket;
    sockaddr_in serverAddr;
};

#endif // NETWORKCLIENT_H

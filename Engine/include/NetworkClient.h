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
    void receiveTurnNotification(std::function<void()> turnCallback);
    void sendTurnComplete();
    void sendPlayerAction(float x, float y, char symbol);

private:
    SOCKET clientSocket;
    sockaddr_in serverAddr;
};

#endif // NETWORKCLIENT_H

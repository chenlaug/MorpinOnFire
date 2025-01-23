#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <winsock2.h>
#include <string>
#include <iostream>

class NetworkServer {
public:
    NetworkServer();
    ~NetworkServer();

    void startServer(int port);
    void acceptClient(int playerNumber);
    void notifyGameStart();
    bool areClientsConnected() const;

private:
    SOCKET serverSocket;
    SOCKET clientSocket1;
    SOCKET clientSocket2;
    sockaddr_in serverAddr;
    bool clientsConnected;
};

#endif // NETWORKSERVER_H

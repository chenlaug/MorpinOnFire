#include <winsock2.h>
#include <string>
#include <iostream>

class NetworkServer {
public:
    NetworkServer();
    ~NetworkServer();

	void startServer(int port);
	void notifyGameStart();
	bool areClientsConnected() const;
	void acceptClient(int playerNumber);

private:
    SOCKET serverSocket;
    SOCKET clientSocket1; 
    SOCKET clientSocket2;
    sockaddr_in serverAddr;
    bool clientsConnected; 
};


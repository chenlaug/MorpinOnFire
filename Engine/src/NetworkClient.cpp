#include "NetworkClient.h"
#include <ws2tcpip.h>  // Pour inet_pton

NetworkClient::NetworkClient() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);  
    clientSocket = INVALID_SOCKET;
}

NetworkClient::~NetworkClient() {
    closesocket(clientSocket);  
    WSACleanup();  
}

void NetworkClient::connectToServer(const std::string& serverIp, int port) {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed!" << std::endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address/Address not supported!" << std::endl;
        return;
    }

    // Connexion au serveur
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed!" << std::endl;
        return;
    }

    std::cout << "Connected to server!" << std::endl;

    std::thread receiveThread(&NetworkClient::receiveLoop, this);
    receiveThread.detach();
}

void NetworkClient::sendMove(const std::string& move) {
    send(clientSocket, move.c_str(), move.size(), 0);
}

std::string NetworkClient::receiveMove() {
    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        return std::string(buffer, bytesReceived);
    }
    return "";
}

void NetworkClient::receiveLoop() {
    while (true) {
        std::string move = receiveMove();
        if (!move.empty()) {
            std::cout << "Received from server: " << move << std::endl;
        }
    }
}

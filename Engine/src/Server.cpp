#include "Server.h"

NetworkServer::NetworkServer() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);  
    serverSocket = INVALID_SOCKET;
    clientSocket = INVALID_SOCKET;
}

NetworkServer::~NetworkServer() {
    closesocket(clientSocket);  
    closesocket(serverSocket);  
    WSACleanup();  
}

void NetworkServer::startServer(int port) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed!" << std::endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Binding failed!" << std::endl;
        return;
    }

    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        std::cerr << "Listen failed!" << std::endl;
        return;
    }

    std::cout << "Server listening on port " << port << "..." << std::endl;

    std::thread acceptThread(&NetworkServer::acceptClient, this);
    acceptThread.detach(); 
}

void NetworkServer::acceptClient() {
    clientAddrSize = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed!" << std::endl;
        return;
    }

    std::cout << "Client connected!" << std::endl;

    std::thread clientThread(&NetworkServer::handleClient, this);
    clientThread.detach(); 
}

void NetworkServer::handleClient() {
    while (true) {
        std::string move = receiveMove();
        if (!move.empty()) {
            std::cout << "Received move: " << move << std::endl;
            sendMove("OK");  
        }
    }
}

void NetworkServer::sendMove(const std::string& move) {
    send(clientSocket, move.c_str(), move.size(), 0);
}

std::string NetworkServer::receiveMove() {
    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        return std::string(buffer, bytesReceived);
    }
    return "";
}

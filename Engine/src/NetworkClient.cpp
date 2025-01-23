#include "NetworkClient.h"
#include <ws2tcpip.h>

NetworkClient::NetworkClient() {
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Erreur WinSock" << std::endl;
        exit(1);
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Erreur de création du socket" << std::endl;
        exit(1);
    }
}

NetworkClient::~NetworkClient() {
    closesocket(clientSocket);
    WSACleanup();
}

void NetworkClient::connectToServer(const std::string& serverIP, int port) {
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        int errorCode = WSAGetLastError();
        std::cerr << "Erreur de connexion au serveur. Code d'erreur : " << errorCode << std::endl;
        return;
    }

    std::cout << "Connecté au serveur !" << std::endl;
}



void NetworkClient::receiveGameStartNotification(std::function<void()> gameStartCallback) {
    char buffer[512];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

    // Vérifier si la réception a échoué
    if (bytesReceived == SOCKET_ERROR) {
        int errorCode = WSAGetLastError();
        std::cerr << "Erreur de réception du message. Code d'erreur : " << errorCode << std::endl;
        return;
    }

    // Si des données ont été reçues
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';  // Ajouter le caractère de fin de chaîne
        std::cout << "Notification de démarrage du jeu : " << buffer << std::endl;

        // Appeler le callback pour démarrer le jeu
        gameStartCallback();
    }
    else {
        std::cerr << "Aucune donnée reçue ou connexion fermée." << std::endl;
    }
}



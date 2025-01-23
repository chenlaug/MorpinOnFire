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
        std::cerr << "Erreur de cr�ation du socket" << std::endl;
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

    std::cout << "Connect� au serveur !" << std::endl;
}



void NetworkClient::receiveGameStartNotification(std::function<void()> gameStartCallback) {
    char buffer[512];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

    // V�rifier si la r�ception a �chou�
    if (bytesReceived == SOCKET_ERROR) {
        int errorCode = WSAGetLastError();
        std::cerr << "Erreur de r�ception du message. Code d'erreur : " << errorCode << std::endl;
        return;
    }

    // Si des donn�es ont �t� re�ues
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';  // Ajouter le caract�re de fin de cha�ne
        std::cout << "Notification de d�marrage du jeu : " << buffer << std::endl;

        // Appeler le callback pour d�marrer le jeu
        gameStartCallback();
    }
    else {
        std::cerr << "Aucune donn�e re�ue ou connexion ferm�e." << std::endl;
    }
}



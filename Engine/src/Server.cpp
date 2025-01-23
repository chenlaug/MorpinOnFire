#include "Server.h"
#include <iostream>
#include <winsock2.h>

NetworkServer::NetworkServer() : clientSocket1(0), clientSocket2(0), clientsConnected(false) {
    // Initialisation de WinSock
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Erreur de l'initialisation de WinSock" << std::endl;
        exit(1);
    }

    // Cr�er le socket pour le serveur
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Erreur de cr�ation du socket serveur. Code d'erreur : " << WSAGetLastError() << std::endl;
        exit(1);
    }
    else {
        std::cout << "Socket serveur cr�� avec succ�s." << std::endl;
    }
}


NetworkServer::~NetworkServer() {
    if (clientSocket1) {
        closesocket(clientSocket1);
    }
    if (clientSocket2) {
        closesocket(clientSocket2);
    }
    closesocket(serverSocket);
    WSACleanup();
}

void NetworkServer::startServer(int port) {
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Adresse IP du serveur (0.0.0.0 = toutes les adresses)
    serverAddr.sin_port = htons(port);  // Convertir le port en format r�seau

    // Lier le socket � l'adresse
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Erreur de liaison du socket. Code d'erreur : " << WSAGetLastError() << std::endl;
        return;
    }
    else {
        std::cout << "Le socket a �t� li� avec succ�s." << std::endl;
    }

    // Commencer � �couter sur le socket
    if (listen(serverSocket, 2) == SOCKET_ERROR) {
        std::cerr << "Erreur d'�coute sur le socket. Code d'erreur : " << WSAGetLastError() << std::endl;
        return;
    }
    else {
        std::cout << "Le serveur �coute les connexions entrantes." << std::endl;
    }

    // Accepter la connexion du premier client
    acceptClient(1);

    // Accepter la connexion du deuxi�me client
    acceptClient(2);

    // Une fois les deux clients connect�s, notifier le d�marrage du jeu
    if (areClientsConnected()) {
        notifyGameStart();
    }
}

void NetworkServer::acceptClient(int playerNumber) {
    sockaddr_in clientAddr;
    int clientSize = sizeof(clientAddr);

    SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Erreur d'acceptation de la connexion. Code d'erreur : " << WSAGetLastError() << std::endl;
        return;
    }

    // Afficher des informations sur l'acceptation
    std::cout << "Un client s'est connect�." << std::endl;

    // Enregistrer les clients
    if (playerNumber == 1) {
        clientSocket1 = clientSocket;
        std::cout << "Premier joueur connect�." << std::endl;
    }
    else {
        clientSocket2 = clientSocket;
        std::cout << "Deuxi�me joueur connect�." << std::endl;
    }

    clientsConnected = (clientSocket1 != 0 && clientSocket2 != 0);
}

void NetworkServer::notifyGameStart() {
    // V�rifiez que les deux clients sont connect�s avant d'envoyer la notification
    std::cerr << "V�rification de la connexion des clients..." << std::endl;

    if (areClientsConnected()) {
        std::cerr << "Les deux clients sont connect�s. Envoi de la notification..." << std::endl;

        const char* gameStartMessage = "Le jeu commence !";

        // V�rifiez que les sockets sont valides avant de tenter d'envoyer des donn�es
        if (clientSocket1 != INVALID_SOCKET && clientSocket2 != INVALID_SOCKET) {
            send(clientSocket1, gameStartMessage, strlen(gameStartMessage), 0);
            send(clientSocket2, gameStartMessage, strlen(gameStartMessage), 0);
            std::cout << "Notification envoy�e aux clients : Le jeu commence !" << std::endl;
        }
        else {
            std::cerr << "Erreur: Un ou les deux sockets des clients sont invalides !" << std::endl;
        }
    }
    else {
        std::cerr << "Les clients ne sont pas tous connect�s. Impossible d'envoyer la notification de d�marrage." << std::endl;
    }
}

bool NetworkServer::areClientsConnected() const {
    return (clientSocket1 != 0 && clientSocket2 != 0);
}

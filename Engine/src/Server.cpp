#include "Server.h"
#include <iostream>
#include <winsock2.h>
#include <mutex>

std::mutex clientMutex;

NetworkServer::NetworkServer() : clientSocket1(0), clientSocket2(0), clientsConnected(false) {
    // Initialiser WinSock
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Erreur de l'initialisation de WinSock" << std::endl;
        exit(1);
    }

    // Créer le socket pour le serveur
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Erreur de création du socket serveur" << std::endl;
        exit(1);
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
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Adresse IP du serveur
    serverAddr.sin_port = htons(port);  // Port de connexion

    // Lier le socket au port
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Erreur de liaison du socket" << std::endl;
        return;
    }

    std::cout << "Le serveur écoute les connexions entrantes..." << std::endl;

    // Lancer l'écoute sur le serveur
    if (listen(serverSocket, 2) == SOCKET_ERROR) {
        std::cerr << "Erreur d'écoute sur le socket" << std::endl;
        return;
    }

    // Attendre les connexions des clients
    acceptClient(1);  // Accepter le premier client
    acceptClient(2);  // Accepter le deuxième client

    // Une fois les deux clients connectés, notifier le démarrage du jeu
    if (areClientsConnected()) {
        notifyGameStart();
    }
}

void NetworkServer::notifyGameStart() {
    if (clientSocket1 == INVALID_SOCKET || clientSocket2 == INVALID_SOCKET) {
        std::cerr << "Erreur : un ou plusieurs sockets de client ne sont pas valides." << std::endl;
        return;
    }

    const char* gameStartMessage = "Le jeu commence !";
    send(clientSocket1, gameStartMessage, strlen(gameStartMessage), 0);
    send(clientSocket2, gameStartMessage, strlen(gameStartMessage), 0);
    std::cout << "Notification envoyée aux clients : Le jeu commence !" << std::endl;
}

bool NetworkServer::areClientsConnected() const {
    return (clientSocket1 != 0 && clientSocket2 != 0);
}

void NetworkServer::acceptClient(int playerNumber) {
    sockaddr_in clientAddr;
    int clientSize = sizeof(clientAddr);

    // Accepter la connexion d'un client
    SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Erreur d'acceptation de la connexion. Code d'erreur : " << WSAGetLastError() << std::endl;
        return;
    }

    // Enregistrer le premier ou le deuxième client
    if (playerNumber == 1) {
        clientSocket1 = clientSocket;
        std::cout << "Premier joueur connecté." << std::endl;
    }
    else {
        clientSocket2 = clientSocket;
        std::cout << "Deuxième joueur connecté." << std::endl;
    }

    clientsConnected = (clientSocket1 != 0 && clientSocket2 != 0);
}



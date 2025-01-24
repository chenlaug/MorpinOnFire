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
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

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

    fd_set readfds;
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000;  // Timeout pour select()

    // Attendre les connexions des clients
    acceptClient(1);
    acceptClient(2);

    if (areClientsConnected()) {
        notifyGameStart();
    }

    // Commencer à gérer les tours
    while (true) {
        FD_ZERO(&readfds);
        FD_SET(clientSocket1, &readfds);
        FD_SET(clientSocket2, &readfds);

        // Utilisation de select pour surveiller les sockets
        int activity = select(0, &readfds, NULL, NULL, &timeout);

        if (activity > 0) {
            if (FD_ISSET(clientSocket1, &readfds)) {
                // Traiter le message du client 1
                handleClientMessage(clientSocket1);
            }

            if (FD_ISSET(clientSocket2, &readfds)) {
                // Traiter le message du client 2
                handleClientMessage(clientSocket2);
            }
        }

        // Processus du tour
        processTurn();
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

void NetworkServer::notifyTurnToClient(int playerNumber) {
    const char* turnMessage = (playerNumber == 1) ? "C'est votre tour, Joueur 1 !" : "C'est votre tour, Joueur 2 !";
    send(clientSocket1, turnMessage, strlen(turnMessage), 0);
    send(clientSocket2, turnMessage, strlen(turnMessage), 0);
}

void NetworkServer::processTurn() {
    static int currentPlayer = 1;  // Pour suivre le joueur actuel (1 ou 2)

    // Envoyer la notification du tour au client actuel
    notifyTurnToClient(currentPlayer);

    // Attendre l'action du joueur (coordonnées du clic et symbole)
    char buffer[512];
    SOCKET currentSocket = (currentPlayer == 1) ? clientSocket1 : clientSocket2;

    int bytesReceived = recv(currentSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';

        // Extraire les coordonnées et le symbole envoyés
        float x, y;
        char symbol;
        sscanf(buffer, "%f %f %c", &x, &y, &symbol);

        // Notifier les autres clients de l'action
        notifyClientsOfPlayerAction(x, y, symbol);

        // Passer au joueur suivant pour le prochain tour
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }
}

void NetworkServer::notifyClientsOfPlayerAction(float x, float y, char symbol) {
    std::string message = std::to_string(x) + " " + std::to_string(y) + " " + symbol;

    // Envoyer à chaque client
    send(clientSocket1, message.c_str(), message.size(), 0);
    send(clientSocket2, message.c_str(), message.size(), 0);
}

void NetworkServer::handleClientMessage(SOCKET clientSocket) {
    // Recevoir un message du client
    char buffer[512];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Message reçu : " << buffer << std::endl;
        // Vous pouvez ici gérer les messages des clients
    }
    else {
        // Si le client est déconnecté, vous pouvez le fermer
        if (bytesReceived == 0) {
            std::cout << "Le client a fermé la connexion." << std::endl;
        }
        else {
            std::cerr << "Erreur lors de la réception du message. Code d'erreur : " << WSAGetLastError() << std::endl;
        }
        closesocket(clientSocket);
    }
}
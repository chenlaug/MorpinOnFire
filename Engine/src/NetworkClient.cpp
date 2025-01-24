#include "NetworkClient.h"

#include <string>
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
	try {
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(port);
		inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr);

		if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
			int errorCode = WSAGetLastError();
			std::cerr << "Erreur de connexion au serveur. Code d'erreur : " << errorCode << std::endl;
			exit(1);
		}

		std::cout << "Connecté au serveur !" << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception lors de la connexion : " << e.what() << std::endl;
	}
}

void NetworkClient::receiveGameStartNotification(std::function<void()> gameStartCallback) {
	if (clientSocket == INVALID_SOCKET) {
		std::cerr << "Erreur : le socket client est invalide." << std::endl;
		return;
	}

	char buffer[512];
	int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

	if (bytesReceived == SOCKET_ERROR) {
		int errorCode = WSAGetLastError();
		std::cerr << "Erreur de réception du message. Code d'erreur : " << errorCode << std::endl;
		return;
	}

	if (bytesReceived > 0) {
		buffer[bytesReceived] = '\0';  // Ajouter le caractère de fin de chaîne
		std::cout << "Notification de démarrage du jeu : " << buffer << std::endl;

		// Appeler le callback pour démarrer le jeu
		gameStartCallback();  // Démarre le jeu
	}
	else {
		std::cerr << "Aucun message reçu." << std::endl;
	}
}

void NetworkClient::receiveTurnNotification(std::function<void()> turnCallback) {
	if (clientSocket == INVALID_SOCKET) {
		std::cerr << "Erreur : le socket client est invalide." << std::endl;
		return;
	}

	char buffer[512];
	int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

	if (bytesReceived > 0) {
		buffer[bytesReceived] = '\0';  // Ajouter le caractère de fin de chaîne
		std::cout << "Notification de tour : " << buffer << std::endl;

		if (strcmp(buffer, "C'est votre tour, Joueur 1 !") == 0 || strcmp(buffer, "C'est votre tour, Joueur 2 !") == 0) {
			// Appeler le callback pour signaler que c'est le tour du joueur
			turnCallback();  // Cette fonction sera appelée pour indiquer que le joueur peut jouer

			// Une fois le joueur a joué, envoyer un message au serveur pour signaler que le tour est terminé
			send(clientSocket, "Tour terminé", strlen("Tour terminé"), 0);
		}
	}
}

void NetworkClient::sendTurnComplete() {
	if (clientSocket != INVALID_SOCKET) {
		const char* message = "Tour terminé";
		send(clientSocket, message, strlen(message), 0);
	}
}

void NetworkClient::sendPlayerAction(float x, float y, char symbol) {
	// Créer un message à envoyer (vous pouvez sérialiser les données si nécessaire)
	std::string message = std::to_string(x) + " " + std::to_string(y) + " " + symbol;

	// Envoyer le message au serveur
	send(clientSocket, message.c_str(), message.size(), 0);
}


#include "Game.h"
#include "HumainPlayer.h"
#include "AiPlayer.h"
#include <iostream>
#include <thread>

Game::Game() :
    window(800, 600, "Game"),
    text("../Assets/Font/zai_Smith-CoronaGalaxieTypewriter.otf"),
    board(window.getRenderWindow(), text),
    tour(0),
    turnPlayed(false),
    gameState(GameState::Menu),
    menu(text)
{
}

Game::~Game()
{
    clear();
    std::cout << "Game destructor" << std::endl;
}

void Game::init()
{
    player1 = std::make_unique<HumainPlayer>('L', "Player 1");
    player2 = std::make_unique<AiPlayer>('O', "Player 2");
    dynamic_cast<AiPlayer*>(player2.get())->setOpponentSymbol(player1->getSymbol());
}

void Game::run() {
    while (window.open()) {
        window.clear();
        event.pollEvent(window.getRenderWindow());

        if (gameState == GameState::Menu) {
            handleMenu();  // Afficher le menu si on est en mode Menu
        }
        else if (gameState == GameState::Playing) {
            turnPlayed = false;
            processTurn();

            // Afficher la grille de jeu
            board.draw(window.getRenderWindow(), player1->getSymbol(), player2->getSymbol());
        }

        window.display();  // Afficher la fenêtre mise à jour
    }
}

void Game::handleMenu() {
    std::thread serverThread;

    while (event.hasEvent()) {
        sf::Event currentEvent = event.getNextEvent();

        if (currentEvent.type == sf::Event::Closed) {
            window.close();
        }
        else if (currentEvent.type == sf::Event::MouseButtonPressed) {
            int mode = menu.handleClick(currentEvent.mouseButton.x, currentEvent.mouseButton.y);
            switch (mode) {
            case 1: // Créer une partie (Serveur)
                std::cout << "Starting server..." << std::endl;

                // Démarrer le serveur dans un thread
                serverThread = std::thread(&NetworkServer::startServer, &networkServer, 12345);
                serverThread.detach();

                std::this_thread::sleep_for(std::chrono::seconds(1));  // Attendre que le serveur soit prêt
                networkClient.connectToServer("127.0.0.1", 12345);
                std::cout << "Host connected to self as client..." << std::endl;

                // Initialiser les joueurs
                player1 = std::make_unique<HumainPlayer>('L', "Player 1");
                player2 = std::make_unique<HumainPlayer>('O', "Player 2");
                gameState = GameState::Playing;
                networkServer.notifyGameStart();
                break;

            case 2: // Rejoindre une partie (Client)
                std::cout << "Joining game..." << std::endl;

                // Initialiser les joueurs pour le client
                player1 = std::make_unique<HumainPlayer>('L', "Player 1");
                player2 = std::make_unique<HumainPlayer>('O', "Player 2");

                networkClient.connectToServer("127.0.0.1", 12345);  // Se connecter à l'hôte
                networkClient.receiveGameStartNotification([this]() { startGame(); });
                gameState = GameState::Playing;
                break;

            case 3: // Jouer en solo (IA)
                std::cout << "Playing solo with IA mode..." << std::endl;
                player1 = std::make_unique<HumainPlayer>('X', "Player 1");
                player2 = std::make_unique<AiPlayer>('O', "AI Player");
                dynamic_cast<AiPlayer*>(player2.get())->setOpponentSymbol(player1->getSymbol());
                gameState = GameState::Playing;
                break;

            case 4: // Jouer en solo amis
                std::cout << "Playing solo with friends mode..." << std::endl;
                player1 = std::make_unique<HumainPlayer>('X', "Player 1");
                player2 = std::make_unique<HumainPlayer>('O', "Player 2");
                gameState = GameState::Playing;
                break;

            default:
                break;
            }
        }
    }

    menu.draw(window.getRenderWindow());
}

void Game::startServerAndClient() {
    std::thread serverThread(&NetworkServer::startServer, &networkServer, 12345);
    serverThread.join(); // Attendre que le serveur soit complètement démarré

    // Maintenant que le serveur est prêt, connectons-nous en tant que client
    networkClient.connectToServer("127.0.0.1", 12345);  // Se connecter au serveur
    std::cout << "Connected as client to the host..." << std::endl;

    // Démarrer le jeu une fois connecté
    gameState = GameState::Playing;
}


void Game::startClient() {
    // Se connecter au serveur (l'hôte)
    networkClient.connectToServer("127.0.0.1", 12345);

    // Une fois connecté, attendre le démarrage de la partie
    // Une fois le serveur prêt, le jeu commence
    gameState = GameState::Playing;
}

void Game::startGame() {
    if (!player1 || !player2) {
        std::cerr << "Erreur : les joueurs ne sont pas correctement initialisés !" << std::endl;
        return;
    }

    std::cout << "Démarrage du jeu..." << std::endl;
    gameState = GameState::Playing;
}

void Game::processTurn()
{
    Player* currentPlayer = (getTour() % 2 == 0) ? player1.get() : player2.get();

    if (dynamic_cast<HumainPlayer*>(currentPlayer) && !turnPlayed) {
        while (event.hasEvent()) {
            sf::Event currentEvent = event.getNextEvent();

            if (currentEvent.type == sf::Event::Closed) {
                window.close();
            }
            else if (currentEvent.type == sf::Event::MouseButtonPressed) {
                if (board.handleClick(static_cast<float>(currentEvent.mouseButton.x),
                    static_cast<float>(currentEvent.mouseButton.y),
                    currentPlayer->getSymbol())) {
                    turnPlayed = true;

                    if (checkGameEnd(currentPlayer)) return;
                    setTour();
                }
            }
        }
    }
    else if (dynamic_cast<AiPlayer*>(currentPlayer) && !turnPlayed) {
        currentPlayer->play(window.getRenderWindow(), board, sf::Event());
        turnPlayed = true;

        if (checkGameEnd(currentPlayer)) return;
        setTour();
    }
}

bool Game::checkGameEnd(Player* currentPlayer)
{
    if (board.winCondition(currentPlayer->getSymbol())) {
        std::cout << currentPlayer->getName() << " wins!" << std::endl;
        resetGame();
        return true;
    }

    if (board.drawCondition()) {
        std::cout << "It's a draw!" << std::endl;
        resetGame();
        return true;
    }

    return false;
}

void Game::resetGame()
{
    board.reset();
    tour = 0;
    turnPlayed = false;
    std::cout << "Game has been reset." << std::endl;
}

void Game::clear()
{
    player1.reset();
    player2.reset();
}

void Game::setTour()
{
    tour++;
}

int Game::getTour() const
{
    return tour;
}

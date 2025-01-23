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

void Game::run()
{
    while (window.open()) {
        window.clear();
        event.pollEvent(window.getRenderWindow());

        if (gameState == GameState::Menu) {
            handleMenu();
        } else if (gameState == GameState::Playing) {
            turnPlayed = false;
            processTurn();

            board.draw(window.getRenderWindow(), player1->getSymbol(), player2->getSymbol());
        }

        window.display();
    }
}

void Game::handleMenu()
{
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
                serverThread = std::thread(&NetworkServer::startServer, &networkServer, 12345);
                serverThread.detach(); 
                std::cout << "Server started in the background." << std::endl;
                break;
            case 2: // Rejoindre une partie (Client)
                std::cout << "Joining game..." << std::endl;
                networkClient.connectToServer("127.0.0.1", 12345);
                break;
            case 3: // Jouer en solo (IA)
                std::cout << "Playing solo with ia mode..." << std::endl;
                player1 = std::make_unique<HumainPlayer>('X', "Player 1");
                player2 = std::make_unique<AiPlayer>('O', "AI Player");
                dynamic_cast<AiPlayer*>(player2.get())->setOpponentSymbol(player1->getSymbol());
                gameState = GameState::Playing;
                break;
			case 4: // Jouer en solo friends
                std::cout << "Playing solo with friends mode..." << std::endl;
                player1 = std::make_unique<HumainPlayer>('X', "Player 1");
                player2 = std::make_unique<HumainPlayer>('O', "AI Player");
                gameState = GameState::Playing;
            default:
                break;
            }
        }
    }

    menu.draw(window.getRenderWindow());
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

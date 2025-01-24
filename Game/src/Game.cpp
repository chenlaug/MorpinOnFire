#include "Game.h"
#include "HumainPlayer.h"
#include "AiPlayer.h"
#include <iostream>

Game::Game() :
	window(800, 600, "Game"),
	text("../Assets/Font/zai_Smith-CoronaGalaxieTypewriter.otf"),
	board(window.getRenderWindow(), text),
	tour(0),
	turnPlayed(false)
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
	init();

	while (window.open()) {
		window.clear();
		event.pollEvent(window.getRenderWindow());

		turnPlayed = false;

		processTurn();

		board.draw(window.getRenderWindow(), player1->getSymbol(), player2->getSymbol());
		window.display();
	}
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

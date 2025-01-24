#pragma once
#include <memory>
#include "Board.h"
#include "Event.h"
#include "Player.h"
#include "Text.h"
#include "Window.h"

class Game
{
private:
	Window window;
	Board board;
	Text text;
	Event event;
	std::unique_ptr<Player> player1;
	std::unique_ptr<Player> player2;
	int tour;
	bool turnPlayed;

	void setTour();
	int getTour() const;
	void processTurn();
	bool checkGameEnd(Player* currentPlayer);

public:
	Game();
	~Game();
	void init();
	void run();
	void clear();
	void resetGame();
};

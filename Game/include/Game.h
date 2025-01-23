#pragma once
#include <memory>
#include "Board.h"
#include "Event.h"
#include "Menu.h"
#include "NetworkClient.h"
#include "Player.h"
#include "Server.h"
#include "Text.h"
#include "Window.h"

enum class GameState {
	Menu,
	Playing
};

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
	GameState gameState;
	Menu menu;
	NetworkServer networkServer;  
	NetworkClient networkClient; 

	void setTour();
	int getTour() const;
	void processTurn();
	bool checkGameEnd(Player* currentPlayer);
	void handleMenu();

public:
	Game();
	~Game();
	void init();
	void run();
	void clear();
	void resetGame();
};

#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "Board.h"

class Player
{
protected:
	char playerSymbol;
	std::string name;


public:
	Player(char symbol, std::string name);
	virtual ~Player() = default;

	void setSymbol(char symbol);
	void setName(std::string name);
	char getSymbol() const;
	std::string getName() const;
	virtual void play(sf::RenderWindow& window, Board& board, const sf::Event& event) = 0;
};

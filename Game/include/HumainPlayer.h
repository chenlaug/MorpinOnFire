#pragma once
#include "Board.h"
#include "Player.h"

class HumainPlayer : public Player
{
public:
	HumainPlayer(char symbol, std::string name);
	~HumainPlayer();
	void play(sf::RenderWindow& window, Board& board, const sf::Event& event) override;
};


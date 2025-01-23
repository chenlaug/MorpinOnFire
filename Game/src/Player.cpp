#include "Player.h"

Player::Player(char symbol, std::string name) : playerSymbol(symbol), name(name)
{
}

void Player::setSymbol(char symbol)
{
	this->playerSymbol = symbol;
}

void Player::setName(std::string name)
{
	this->name = name;
}

char Player::getSymbol() const
{
	return playerSymbol;
}

std::string Player::getName() const
{
	return this->name;
}

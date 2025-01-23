#include "HumainPlayer.h"
#include "Board.h"
#include <iostream>
#include <SFML/Window/Event.hpp> 

HumainPlayer::HumainPlayer(char symbol, std::string name) : Player(symbol, name)
{
}

HumainPlayer::~HumainPlayer()
{
	std::cout << "HumainPlayer destructor" << '\n';
}

void HumainPlayer::play(sf::RenderWindow& window, Board& board, const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        float mouseX = event.mouseButton.x;
        float mouseY = event.mouseButton.y;

        if (board.handleClick(mouseX, mouseY, getSymbol())) {
            std::cout << getName() << " a joué en (" << mouseX << ", " << mouseY << ")" << std::endl;
        }
        else {
            std::cout << "Coup invalide. Essayez à nouveau !" << std::endl;
        }
    }
}


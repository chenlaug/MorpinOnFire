#include "Text.h"

#include <iostream>

Text::Text(std::string pathFont)
{
	if (!font.loadFromFile(pathFont))
	{
		std::cerr << "Impossible de charger la police" << pathFont << '\n';
	}
	text.setFont(font);
}

Text::~Text()
{
	std::cout << "Text destructor" << '\n';
}

void Text::setText(const std::string& str)
{
	text.setString(str);
}

void Text::setCharacterSize(unsigned int size)
{
	text.setCharacterSize(size);
}

void Text::setColor(const sf::Color& color)
{
	text.setFillColor(color);
}

void Text::setPosition(float x, float y)
{
	text.setPosition(x, y);
}

void Text::setStyle(sf::Text::Style style)
{
	text.setStyle(style);
}

void Text::draw(sf::RenderWindow& window)
{
	window.draw(text);
}
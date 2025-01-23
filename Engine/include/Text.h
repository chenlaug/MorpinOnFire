#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class Text
{
private:
	sf::Text text;
	sf::Font font;

public:
	Text(std::string pathFont);
	~Text();

	void setText(const std::string& str);
	const sf::Text& getText() const;
	void setCharacterSize(unsigned int size);
	void setColor(const sf::Color& color);
	void setPosition(float x, float y);
	void setStyle(sf::Text::Style style);
	void draw(sf::RenderWindow& window);
};


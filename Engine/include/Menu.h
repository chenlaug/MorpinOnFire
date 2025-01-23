#pragma once
#include <SFML/Graphics.hpp>
#include "Text.h"

class Menu {
private:
    Text& text;
    sf::RectangleShape button1, button2, button3, button4;

public:
    Menu(Text& text);
    void draw(sf::RenderWindow& window);
    int handleClick(float x, float y);
};

#include "Menu.h"

Menu::Menu(Text& text) : text(text) {
    // Initialisation des boutons
    button1.setSize(sf::Vector2f(200.f, 50.f));
    button1.setPosition(300.f, 200.f);
    button1.setFillColor(sf::Color::Blue);

    button2.setSize(sf::Vector2f(200.f, 50.f));
    button2.setPosition(300.f, 300.f);
    button2.setFillColor(sf::Color::Green);

    button3.setSize(sf::Vector2f(200.f, 50.f));
    button3.setPosition(300.f, 400.f);
    button3.setFillColor(sf::Color::Red);

	button4.setSize(sf::Vector2f(200.f, 50.f));
	button4.setPosition(300.f, 500.f);
	button4.setFillColor(sf::Color::Cyan);
}

void Menu::draw(sf::RenderWindow& window) {
    window.draw(button1);
    window.draw(button2);
    window.draw(button3);
	window.draw(button4);

    text.setText("Create");
    text.setColor(sf::Color::White);
    sf::FloatRect textBounds = text.getText().getLocalBounds();
    text.setPosition(button1.getPosition().x + (button1.getSize().x - textBounds.width) / 2,
        button1.getPosition().y + (button1.getSize().y - textBounds.height) / 2);
    text.draw(window);

    text.setText("Join");
    text.setColor(sf::Color::White); 
    textBounds = text.getText().getLocalBounds();
    text.setPosition(button2.getPosition().x + (button2.getSize().x - textBounds.width) / 2,
        button2.getPosition().y + (button2.getSize().y - textBounds.height) / 2);
    text.draw(window);

    text.setText("With IA");
    text.setColor(sf::Color::White); 
    textBounds = text.getText().getLocalBounds();
    text.setPosition(button3.getPosition().x + (button3.getSize().x - textBounds.width) / 2,
        button3.getPosition().y + (button3.getSize().y - textBounds.height) / 2);
    text.draw(window);

	text.setText("Multi lan");
	text.setColor(sf::Color::White);
	textBounds = text.getText().getLocalBounds();
	text.setPosition(button4.getPosition().x + (button4.getSize().x - textBounds.width) / 2,
		button4.getPosition().y + (button4.getSize().y - textBounds.height) / 2);
	text.draw(window);
}

int Menu::handleClick(float x, float y) {
    if (button1.getGlobalBounds().contains(x, y)) {
        return 1; // Créer une partie
    }
    if (button2.getGlobalBounds().contains(x, y)) {
        return 2; // Rejoindre une partie
    }
    if (button3.getGlobalBounds().contains(x, y)) {
        return 3; // Jouer en solo
    }
    if (button4.getGlobalBounds().contains(x, y)) {
        return 4; // Jouer en solo
    }
    return 0; // Aucun bouton cliqué
}

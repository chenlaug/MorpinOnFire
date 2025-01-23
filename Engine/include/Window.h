#pragma once
#include <string>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

class Window
{
private:
	int width;
	int height;
	std::string title;
	sf::RenderWindow window;
	sf::View view;
	bool isOpen;
	bool isFullscreen;

public:
	Window(int width, int height, const std::string& title);
	~Window();
	void clear();
	void display();
	void close();
	void update();
	bool open();
	bool pollEvent(sf::Event&);
	sf::RenderWindow& getRenderWindow();
	sf::Vector2u getSize();
	void handleResize(const sf::Event& event);
	void setVSync(bool b);
	void toggleFullscreen();
};
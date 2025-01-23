#pragma once
#include <queue>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class Event
{
private:
	std::queue<sf::Event> events;
public:
	void pollEvent(sf::RenderWindow& window);
	bool hasEvent() const;
	void clear();
	sf::Event getNextEvent();
};


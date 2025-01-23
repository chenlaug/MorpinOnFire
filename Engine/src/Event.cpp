#include "Event.h"

void Event::pollEvent(sf::RenderWindow& window)
{
	sf::Event event;
	while (window.pollEvent(event)) {
		events.push(event);
	}
}

bool Event::hasEvent() const
{
	return !events.empty();
}

void Event::clear()
{
	while (!events.empty()) {
		events.pop();
	}
}

sf::Event Event::getNextEvent()
{
	if (events.empty()) {
		throw std::runtime_error("No events in queue");
	}
	sf::Event event = events.front();
	events.pop();
	return event;
}


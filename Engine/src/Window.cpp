#include "Window.h"
#include <SFML/Window/Event.hpp>
#include <iostream>

Window::Window(int width, int height, const std::string& title)
	: width(width), height(height), title(title),
	window(sf::VideoMode(width, height), title),
	isOpen(true),
	isFullscreen(false) {
	view = sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(width), static_cast<float>(height)));
	window.setView(view);
}

Window::~Window()
{
	if (isOpen) {
		window.close();
	}
	std::cout << "Window destroyed" << '\n';
}

void Window::display()
{
	window.display();
}

void Window::close()
{
	window.close();
	isOpen = false;
}

void Window::clear() {
	window.clear(sf::Color::Black);
}

void Window::update()
{

}

bool Window::open()
{
	return isOpen;
}

bool Window::pollEvent(sf::Event& event) {
	return window.pollEvent(event);
}

sf::RenderWindow& Window::getRenderWindow() {
	return window;
}

sf::Vector2u Window::getSize()
{
	return window.getSize();
}

void Window::handleResize(const sf::Event& event) {

	float aspectRatio = static_cast<float>(event.size.width) / static_cast<float>(event.size.height);
	if (aspectRatio > 1.f) {
		view.setSize(static_cast<float>(event.size.height) * aspectRatio, static_cast<float>(event.size.height));
	}
	else {
		view.setSize(static_cast<float>(event.size.width), static_cast<float>(event.size.width) / aspectRatio);
	}
	view.setCenter(view.getSize().x / 2.f, view.getSize().y / 2.f);

	window.setView(view);

}

void Window::setVSync(bool b)
{
	window.setVerticalSyncEnabled(b);
}

void Window::toggleFullscreen() {
	isFullscreen = !isFullscreen;
	window.close();

	sf::Uint32 style = isFullscreen ? sf::Style::Fullscreen : sf::Style::Default;
	window.create(sf::VideoMode(isFullscreen ? sf::VideoMode::getDesktopMode() : sf::VideoMode(width, height)), title, style);

	view = sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
	window.setView(view);
	isOpen = true;
}

#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Text.h"

class Board {
private:
	static const int rows = 3;
	static const int cols = 3;

	float gridSize;
	float cellWidth;
	float cellHeight;
	float offsetX;
	float offsetY;
	float lineThickness;

	std::vector<std::vector<char>> grid;
	mutable sf::RectangleShape line;
	Text& text;

	void drawGrid(sf::RenderWindow& window) const;
	void drawSymbols(sf::RenderWindow& window, char player1Symbol, char player2Symbol) const;

public:
	Board(sf::RenderWindow& window, Text& text);
	~Board();

	void resetCell(int row, int col);
	bool handleClick(float x, float y, char symbol);
	bool handleClickByIndex(int row, int col, char symbol);
	bool isValidMove(int row, int col) const;
	void draw(sf::RenderWindow& window, char player1Symbol, char player2Symbol);
	bool winCondition(char symbol) const;
	bool drawCondition() const;
	void reset();
	const std::vector<std::vector<char>>& getGrid() const;
};

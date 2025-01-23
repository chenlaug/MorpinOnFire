#include "Board.h"
#include <iostream>
#include <algorithm>

Board::Board(sf::RenderWindow& window, Text& text)
	: lineThickness(5), text(text) {
	sf::Vector2u windowSize = window.getSize();
	gridSize = static_cast<float>(std::min(windowSize.x, windowSize.y) * 0.8f);
	cellWidth = gridSize / cols;
	cellHeight = gridSize / rows;
	offsetX = (windowSize.x - gridSize) / 2.0f;
	offsetY = (windowSize.y - gridSize) / 2.0f;

	grid = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };

	line.setFillColor(sf::Color::White);
}

Board::~Board() {
}

const std::vector<std::vector<char>>& Board::getGrid() const {
	return grid;
}

void Board::resetCell(int row, int col) {
	if (row >= 0 && row < rows && col >= 0 && col < cols) {
		grid[row][col] = ' ';
	}
}


bool Board::handleClick(float x, float y, char symbol) {
	float localX = x - offsetX;
	float localY = y - offsetY;
	int row = static_cast<int>(localY / cellHeight);
	int col = static_cast<int>(localX / cellWidth);

	return handleClickByIndex(row, col, symbol);
}

bool Board::handleClickByIndex(int row, int col, char symbol) {
	if (row >= 0 && row < rows && col >= 0 && col < cols && grid[row][col] == ' ') {
		grid[row][col] = symbol;
		return true;
	}
	return false;
}

bool Board::isValidMove(int row, int col) const {
	return row >= 0 && row < rows && col >= 0 && col < cols && grid[row][col] == ' ';
}


void Board::draw(sf::RenderWindow& window, char player1Symbol, char player2Symbol) {
	drawGrid(window);
	drawSymbols(window, player1Symbol, player2Symbol);
}

void Board::drawGrid(sf::RenderWindow& window) const {
	for (int i = 1; i < rows; i++) {
		line.setSize({ gridSize, lineThickness });
		line.setPosition(offsetX, offsetY + i * cellHeight - lineThickness / 2);
		window.draw(line);
	}

	for (int i = 1; i < cols; i++) {
		line.setSize({ lineThickness, gridSize });
		line.setPosition(offsetX + i * cellWidth - lineThickness / 2, offsetY);
		window.draw(line);
	}
}

void Board::drawSymbols(sf::RenderWindow& window, char player1Symbol, char player2Symbol) const {
	text.setCharacterSize(static_cast<unsigned int>(std::min(cellWidth, cellHeight) * 0.7f));

	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			if (grid[row][col] != ' ') {
				if (grid[row][col] == player1Symbol) {
					text.setColor(sf::Color::Red);
				}
				else if (grid[row][col] == player2Symbol) {
					text.setColor(sf::Color::Blue);
				}
				else {
					text.setColor(sf::Color::White);
				}

				text.setText(std::string(1, grid[row][col]));
				text.setPosition(
					offsetX + static_cast<float>(col) * cellWidth + cellWidth * 0.25f,
					offsetY + static_cast<float>(row) * cellHeight + cellHeight * 0.1f
				);
				text.draw(window);
			}
		}
	}
}


bool Board::winCondition(char symbol) const {
	for (int i = 0; i < rows; i++) {
		if ((grid[i][0] == symbol && grid[i][1] == symbol && grid[i][2] == symbol) ||
			(grid[0][i] == symbol && grid[1][i] == symbol && grid[2][i] == symbol)) {
			return true;
		}
	}

	if ((grid[0][0] == symbol && grid[1][1] == symbol && grid[2][2] == symbol) ||
		(grid[0][2] == symbol && grid[1][1] == symbol && grid[2][0] == symbol)) {
		return true;
	}

	return false;
}


bool Board::drawCondition() const {
	for (const auto& row : grid) {
		for (char cell : row) {
			if (cell == ' ') return false;
		}
	}
	return true;
}

void Board::reset() {
	for (auto& row : grid) {
		std::fill(row.begin(), row.end(), ' ');
	}
}

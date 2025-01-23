#include "AiPlayer.h"
#include <algorithm>
#include <limits>


AiPlayer::AiPlayer(char symbol, std::string name) : Player(symbol, name), opponentSymbol(' ') {}

AiPlayer::~AiPlayer() {}

int AiPlayer::evaluate(const Board& board, char aiSymbol, char humanSymbol) const {
	if (board.winCondition(aiSymbol)) return 10;
	if (board.winCondition(humanSymbol)) return -10;

	const auto& grid = board.getGrid();
	int score = 0;

	if (grid[1][1] == aiSymbol) score += 3;
	else if (grid[1][1] == humanSymbol) score -= 3;

	static const std::vector<std::pair<int, int>> corners = { {0, 0}, {0, 2}, {2, 0}, {2, 2} };
	for (const auto& corner : corners) {
		int row = corner.first;
		int col = corner.second;
		score += (grid[row][col] == aiSymbol) ? 2 : (grid[row][col] == humanSymbol ? -2 : 0);
	}

	return score;
}

bool AiPlayer::isMovesLeft(const Board& board) const {
	const auto& grid = board.getGrid();
	for (const auto& row : grid) {
		if (std::any_of(row.begin(), row.end(), [](char cell) { return cell == ' '; })) {
			return true;
		}
	}
	return false;
}

int AiPlayer::minimax(Board& board, int depth, bool isMax, char aiSymbol, char humanSymbol, int alpha, int beta) {
	int score = evaluate(board, aiSymbol, humanSymbol);

	if (score == 10 || score == -10 || !isMovesLeft(board) || depth == maxDepth) {
		return score;
	}

	if (isMax) {
		int best = std::numeric_limits<int>::min();
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (board.isValidMove(i, j)) {
					board.handleClickByIndex(i, j, aiSymbol);
					best = std::max(best, minimax(board, depth + 1, false, aiSymbol, humanSymbol, alpha, beta));
					board.resetCell(i, j);
					alpha = std::max(alpha, best);
					if (beta <= alpha) return best;
				}
			}
		}
		return best;
	}
	else {
		int best = std::numeric_limits<int>::max();
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (board.isValidMove(i, j)) {
					board.handleClickByIndex(i, j, humanSymbol);
					best = std::min(best, minimax(board, depth + 1, true, aiSymbol, humanSymbol, alpha, beta));
					board.resetCell(i, j);
					beta = std::min(beta, best);
					if (beta <= alpha) return best;
				}
			}
		}
		return best;
	}
}

Move AiPlayer::findBestMove(Board board, char aiSymbol, char humanSymbol) {
	Move bestMove = { -1, -1 };
	int bestValue = std::numeric_limits<int>::min();

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (board.isValidMove(i, j)) {
				board.handleClickByIndex(i, j, aiSymbol);
				int moveValue = minimax(board, 0, false, aiSymbol, humanSymbol,
					std::numeric_limits<int>::min(),
					std::numeric_limits<int>::max());
				board.resetCell(i, j);

				if (moveValue > bestValue) {
					bestMove = { i, j };
					bestValue = moveValue;
				}
			}
		}
	}
	return bestMove;
}

void AiPlayer::setOpponentSymbol(char symbol) {
	opponentSymbol = symbol;
}


void AiPlayer::play(sf::RenderWindow& window, Board& board, const sf::Event& event) {
	char aiSymbol = getSymbol();
	Move bestMove = findBestMove(board, aiSymbol, opponentSymbol);

	if (bestMove.row >= 0 && bestMove.col >= 0) {
		if (board.handleClickByIndex(bestMove.row, bestMove.col, aiSymbol)) {
		}
	}

}

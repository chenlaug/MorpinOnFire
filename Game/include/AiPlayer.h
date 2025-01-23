#pragma once
#include "Player.h"
#include "Board.h"

struct Move {
    int row, col;
};

class AiPlayer : public Player {
private:
    char opponentSymbol;
    static constexpr int MIN_VAL = std::numeric_limits<int>::min();
    static constexpr int MAX_VAL = std::numeric_limits<int>::max();
	const int maxDepth = 9;

    int evaluate(const Board& board, char aiSymbol, char humanSymbol) const;
    bool isMovesLeft(const Board& board) const;
    int minimax(Board& board, int depth, bool isMax, char aiSymbol, char humanSymbol, int alpha, int beta);
    Move findBestMove(Board board, char aiSymbol, char humanSymbol);

public:
    AiPlayer(char symbol, std::string name);
    ~AiPlayer();

    void setOpponentSymbol(char symbol);
    void play(sf::RenderWindow& window, Board& board, const sf::Event& event) override;
};

#pragma once
#include <vector>
#include <string>
#include "TicTacToe.h"

struct PlayerMove
{
    std::string PlayerName;
    TicTacToe::Piece PlayerPiece;
    unsigned int BoardCell;
};

struct GameData
{
public:

    GameData(const std::vector<const PlayerMove*>* allMoves);
    ~GameData();

    std::string GetWinnerName() const { return AllMoves->back()->PlayerName; }
    TicTacToe::Piece GetWinnerPiece() const { return AllMoves->back()->PlayerPiece; }

    const std::vector<const PlayerMove*>* GetMoves() const { return AllMoves; }
    const PlayerMove* GetMove(unsigned int moveIndex) const { return AllMoves->at(moveIndex); }
    size_t GetMovesSize() const { return AllMoves->size(); }

private:

    const std::vector<const PlayerMove*>* AllMoves;
};

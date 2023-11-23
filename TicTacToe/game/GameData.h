#pragma once
#include <vector>
#include <string>
#include "TicTacToe.h"
#include "../tcp-ip/ISerializable.h"

struct PlayerMove : ISerializable
{
    PlayerMove(const std::string& playerName, const TicTacToe::Piece piece, const unsigned int cell) : PlayerName(playerName), PlayerPiece(piece), BoardCell(cell) {}
    PlayerMove(const Json& j) : PlayerName(j["PlayerName"]), PlayerPiece(j["PlayerPiece"]), BoardCell(j["BoardCell"]) {}

    Json Serialize() override;

    std::string PlayerName;
    TicTacToe::Piece PlayerPiece;
    unsigned int BoardCell;
};

struct GameData : ISerializable
{
    GameData() = default;
    GameData(const std::vector<PlayerMove>& allMoves);
    GameData(const Json& j);

    std::string GetWinnerName() const { return AllMoves.back().PlayerName; }
    TicTacToe::Piece GetWinnerPiece() const { return AllMoves.back().PlayerPiece; }

    const std::vector<PlayerMove>& GetMoves() { return AllMoves; }
    const PlayerMove& GetMove(unsigned int moveIndex) const { return AllMoves.at(moveIndex); }
    size_t GetMovesSize() const { return AllMoves.size(); }

    Json Serialize() override;

private:

    std::vector<PlayerMove> AllMoves;
};

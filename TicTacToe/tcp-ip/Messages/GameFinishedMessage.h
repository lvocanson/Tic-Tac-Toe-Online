#pragma once
#include "../ISerializable.h"
#include "../../game/TicTacToe.h"

struct GameFinishedMessage : ISerializable
{
    GameFinishedMessage() = default;
    GameFinishedMessage(const std::string& winnerName, const TicTacToe::Piece piece, unsigned int lobbyID) : WinnerName(winnerName), Piece(piece), LobbyID(lobbyID) {}

    Json Serialize() override;
    void Deserialize(const Json& obj) override;

    std::string WinnerName;
    unsigned int LobbyID;
    TicTacToe::Piece Piece;
};


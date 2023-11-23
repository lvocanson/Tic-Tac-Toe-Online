#pragma once
#include "../ISerializable.h"
#include "../../game/TicTacToe.h"

struct PlayerMoveMessage : ISerializable
{
    PlayerMoveMessage() = default;
    PlayerMoveMessage(const std::string& player, int cell, int lobbyID, TicTacToe::Piece piece) : PlayerName(player), Cell(cell), LobbyID(lobbyID), Piece(piece) { }

    Json Serialize() override;
    void Deserialize(const Json& j) override;

    std::string PlayerName;
    int Cell, LobbyID;
    TicTacToe::Piece Piece;
};


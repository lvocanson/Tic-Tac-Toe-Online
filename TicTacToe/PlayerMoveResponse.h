#pragma once
#include "tcp-ip/ISerializable.h"
#include "game/TicTacToe.h"

struct PlayerMoveResponse : public ISerializable
{
    PlayerMoveResponse() = default;
    PlayerMoveResponse(unsigned int cell, TicTacToe::Piece piece) : Cell(cell), Piece(piece) {}

    Json Serialize() override;
    void Deserialize(const Json& j) override;

    unsigned int Cell;
    TicTacToe::Piece Piece;
};


#pragma once
#include "../ISerializable.h"

struct PlayerMoveMessage : ISerializable
{
    PlayerMoveMessage() = default;
    PlayerMoveMessage(const std::string& player, int cell, int lobbyID) : PlayerName(player), Cell(cell), LobbyID(lobbyID) { }

    Json Serialize() override;
    void Deserialize(Json j) override;

    std::string PlayerName;
    int Cell, LobbyID;
};


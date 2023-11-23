#pragma once
#include "../ISerializable.h"

struct LobbyFullMessage : ISerializable
{
    LobbyFullMessage();
    LobbyFullMessage(int ID, const std::string& playerX, const std::string& playerO) : ID(ID), PlayerX(playerX), PlayerO(playerO) { }

    Json Serialize() override;
    void Deserialize(const Json& j) override;

    int ID;
    std::string PlayerX, PlayerO, StartingPlayer;
};


#pragma once
#include "../ISerializable.h"

struct GameFinishedMessage : ISerializable
{
    GameFinishedMessage() = default;
    GameFinishedMessage(const std::string& winnerName, unsigned int lobbyID) : WinnerName(winnerName), LobbyID(lobbyID) {}

    Json Serialize() override;
    void Deserialize(const Json& obj) override;

    std::string WinnerName;
    unsigned int LobbyID;
};


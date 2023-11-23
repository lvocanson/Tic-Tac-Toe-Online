#pragma once
#include "../ISerializable.h"

struct IsLobbyFullNotification : ISerializable
{
    IsLobbyFullNotification() = default;
    IsLobbyFullNotification(unsigned int lobbyID) : LobbyID(lobbyID) {}

    Json Serialize() override;
    void Deserialize(const Json& j) override;

    unsigned int LobbyID = false;
};


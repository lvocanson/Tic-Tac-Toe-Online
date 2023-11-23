#pragma once
#include "../../game/Lobby.h"
#include "../ISerializable.h"


struct GetLobbyListMessage : ISerializable
{
    GetLobbyListMessage() = default;
    GetLobbyListMessage(const std::vector<Lobby>& lobbyIDs) : AllLobbies(lobbyIDs) { }

    Json Serialize() override;
    void Deserialize(Json j) override;

    std::vector<Lobby> AllLobbies;
};


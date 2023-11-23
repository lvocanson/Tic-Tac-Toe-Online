#pragma once
#include "../../game/Lobby.h"
#include "../ISerializable.h"


struct GetLobbyListMessage : ISerializable
{
    GetLobbyListMessage() = default;
    GetLobbyListMessage(const std::vector<Lobby*>& lobbyIDs);

    Json Serialize() override;
    void Deserialize(const Json& j) override;

    std::vector<LobbyData> AllLobbiesData;
};


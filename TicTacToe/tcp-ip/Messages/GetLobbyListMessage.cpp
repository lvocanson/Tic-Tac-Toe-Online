#include "GetLobbyListMessage.h"

GetLobbyListMessage::GetLobbyListMessage(const std::vector<Lobby*>& lobbyIDs)
{
    for (auto& lobby : lobbyIDs)
    {
        AllLobbiesData.emplace_back(lobby->Data);
    }
}

Json GetLobbyListMessage::Serialize()
{
    Json lobbyListJson;
    lobbyListJson["Type"] = "Lobby";

    for (auto& lobby : AllLobbiesData)
    {
        lobbyListJson["Lobbies"].push_back(lobby.Serialize());
    }

    return lobbyListJson;
}

void GetLobbyListMessage::Deserialize(const Json& j)
{
    for (const auto& lobby : j["Lobbies"])
    {
        LobbyData l;
        l.Deserialize(lobby);
        AllLobbiesData.push_back(l);
    }
}


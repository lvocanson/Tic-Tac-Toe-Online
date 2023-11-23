#include "GetLobbyListMessage.h"

Json GetLobbyListMessage::Serialize()
{
    Json lobbyListJson;
    lobbyListJson["Type"] = "Lobby";

    for (auto& lobby : AllLobbies)
    {
        lobbyListJson["Lobbies"].push_back(lobby.Serialize());
    }

    return lobbyListJson;
}

void GetLobbyListMessage::Deserialize(Json j)
{
    for (const auto& lobby : j["Lobbies"])
    {
        Lobby l;
        l.Deserialize(lobby);
        AllLobbies.push_back(l);
    }
}


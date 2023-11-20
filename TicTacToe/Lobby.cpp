#include "Lobby.h"

Json Lobby::Serialize()
{
    Json j;
    j["ID"] = ID;
    j["PlayerCount"] = PlayerCount;
    return j;
}

void Lobby::Deserialize(Json j)
{
    ID = j["ID"];
    PlayerCount = j["PlayerCount"];

}

void Lobby::AddPlayerToLobby(ClientPtr playerConnection)
{
    PlayerConnections.emplace_back(playerConnection);
}


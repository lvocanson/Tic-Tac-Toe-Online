#include "Lobby.h"

Json Lobby::Serialize()
{
    Json j;
    j["ID"] = ID;
    j["PlayerX"] = PlayerX;
    j["PlayerO"] = PlayerO;
    return j;
}

void Lobby::Deserialize(Json j)
{
    ID = j["ID"];
    PlayerO = j["PlayerO"];
    PlayerX = j["PlayerX"];
}

void Lobby::AddPlayerToLobby(ClientPtr player)
{
    if (PlayerX.empty())
        PlayerX = player->GetName();
    else if (PlayerO.empty())
        PlayerO = player->GetName();
}


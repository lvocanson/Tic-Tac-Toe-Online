#include "Lobby.h"
#include "IDGenerator.h"

Lobby::Lobby()
    : ID(IDGenerator::GenerateLobbyID())
    , PlayerX()
    , PlayerO()
{
}

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

void Lobby::AddPlayerToLobby(const std::string& name)
{
    if (PlayerX.empty())
        PlayerX = name;
    else if (PlayerO.empty())
        PlayerO = name;
}

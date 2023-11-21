#include "Lobby.h"
#include "IDGenerator.h"

Lobby::Lobby()
    : ID(IDGenerator::GenerateLobbyID())
    , PlayerX()
    , PlayerO()
{
}

Lobby::Lobby(const std::string& playerX, const std::string& playerO)
    : ID(IDGenerator::GenerateLobbyID())
    , PlayerX(playerX)
    , PlayerO(playerO)
{
}

Lobby::Lobby(int id, const std::string& playerX, const std::string& playerO)
    : ID(id)
    , PlayerX(playerX)
    , PlayerO(playerO)
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

void Lobby::RemovePlayerFromLobby(const std::string& name)
{
    if (!PlayerX.empty())
        PlayerX = "None";
    else if (!PlayerO.empty())
        PlayerO = "None";
}
#include "Lobby.h"
#include "IDGenerator.h"

Lobby::Lobby()
    : ID(IDGenerator::GenerateLobbyID())
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

std::string& Lobby::GetOpponentName(const std::string& senderName)
{
    if (PlayerO == senderName)
        return PlayerX;
    else if (PlayerX == senderName)
        return PlayerO;

    throw std::exception("Player not found");
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
    if (PlayerX == name)
        PlayerX = "";
    else if (PlayerO == name)
        PlayerO = "";
}

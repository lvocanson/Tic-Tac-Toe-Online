#include "Lobby.h"
#include "IDGenerator.h"

Lobby::Lobby()
{
    Data.ID = IDGenerator::GenerateLobbyID();
    Data.PlayerX = "";
    Data.PlayerO = "";
}

Lobby::Lobby(const std::string& playerX, const std::string& playerO)
{
    Data.ID = IDGenerator::GenerateLobbyID(),
    Data.PlayerX = playerX;
    Data.PlayerO = playerO;
}

Lobby::Lobby(int id, const std::string& playerX, const std::string& playerO)
{
    Data.ID = id;
    Data.PlayerX = playerX;
    Data.PlayerO = playerO;
}

std::string& Lobby::GetOpponentName(const std::string& senderName)
{
    if (Data.PlayerO == senderName)
        return Data.PlayerX;
    else if (Data.PlayerX == senderName)
        return Data.PlayerO;

    throw std::exception("Player not found");
}

void Lobby::AddPlayerToLobby(const std::string& name)
{
    if (Data.PlayerX.empty())
    {
        Data.PlayerX = name;
        PlayerCount++;
    }
    else if (Data.PlayerO.empty())
    {
        Data.PlayerO = name;
        PlayerCount++;
    }
}

void Lobby::RemovePlayerFromLobby(const std::string& name)
{
    if (Data.PlayerX == name)
    {
        Data.PlayerX = "";
        PlayerCount--;
    }
    else if (Data.PlayerO == name)
    {
        Data.PlayerO = "";
        PlayerCount--;
    }
}

LobbyData::LobbyData(const int id, const std::string& playerX, const std::string& playerO)
{
    ID = id;
    PlayerX = playerX;
    PlayerO = playerO;
}

Json LobbyData::Serialize()
{
    Json j;
    j["ID"] = ID;
    j["PlayerX"] = PlayerX;
    j["PlayerO"] = PlayerO;
    return j;
}

void LobbyData::Deserialize(const Json& j)
{
    ID = j["ID"];
    PlayerO = j["PlayerO"];
    PlayerX = j["PlayerX"];
}

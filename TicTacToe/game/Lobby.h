#pragma once
#include <string>
#include "../tcp-ip/ISerializable.h"

struct LobbyData : ISerializable
{
    LobbyData() = default;
    LobbyData(const int id, const std::string& playerX, const std::string& playerO);

    Json Serialize() override;
    void Deserialize(const Json& j) override;

    int ID = -1;
    std::string PlayerX, PlayerO;
};

struct Lobby
{
    Lobby();
    Lobby(const std::string& playerX, const std::string& playerO);
    Lobby(const int id, const std::string& playerX, const std::string& playerO);
    ~Lobby() = default;

    std::string& GetOpponentName(const std::string&);
    void AddPlayerToLobby(const std::string& name);
    void RemovePlayerFromLobby(const std::string& name);

    unsigned int GetPlayerCount() const { return PlayerCount;}

    bool IsInLobby(const std::string& name) const { return Data.PlayerX == name || Data.PlayerO == name; }
    bool IsLobbyFull() const {  return !Data.PlayerX.empty() && !Data.PlayerO.empty(); }
    bool IsLobbyEmpty() const { return Data.PlayerX.empty() && Data.PlayerO.empty(); }

    unsigned int PlayerCount = 0;
    LobbyData Data;
};
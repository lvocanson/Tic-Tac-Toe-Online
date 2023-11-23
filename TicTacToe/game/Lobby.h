#pragma once
#include <string>
#include "../tcp-ip/ISerializable.h"

struct Lobby : ISerializable
{
    Json Serialize() override;
    void Deserialize(Json j) override;

    Lobby();
    Lobby(const std::string& playerX, const std::string& playerO);
    Lobby(const int id, const std::string& playerX, const std::string& playerO);
    ~Lobby() = default;

    std::string& GetOpponentName(const std::string&);
    void AddPlayerToLobby(const std::string& name);
    void RemovePlayerFromLobby(const std::string& name);

    bool IsInLobby(const std::string& name) const
    {
        return PlayerX == name || PlayerO == name;
    }

    bool IsLobbyFull() const
    {
        return !PlayerX.empty() && !PlayerO.empty();
    }

    bool IsLobbyEmpty() const
    {
        return PlayerX.empty() && PlayerO.empty();
    }

    int ID;
    std::string PlayerX, PlayerO;
};

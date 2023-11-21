#pragma once
#include <string>
#include "ISerializable.h"

struct Lobby : ISerializable
{
    Json Serialize() override;
    void Deserialize(Json j) override;

    Lobby();
    ~Lobby() = default;

    void AddPlayerToLobby(const std::string& name);
    bool IsInLobby(const std::string& name)
    {
        return PlayerX == name || PlayerO == name;
    }

    int ID;
    std::string PlayerX, PlayerO;
};

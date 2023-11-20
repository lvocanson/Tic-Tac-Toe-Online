#pragma once
#include <string>
#include <vector>
#include "ISerializable.h"

struct Lobby : ISerializable
{
    Json Serialize() override;
    void Deserialize(Json j) override;

    Lobby();
    Lobby(int id, int playercount, int maxPlayercount)
        : ID(id)
        , PlayerCount(playercount)
        , MaxPlayerCount(maxPlayercount)
    {
    }
    ~Lobby(){}

    int ID;
    int PlayerCount;
    int MaxPlayerCount;
};

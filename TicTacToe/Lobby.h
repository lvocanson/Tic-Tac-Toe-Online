#pragma once
#include <string>
#include <vector>
#include "ISerializable.h"
#include "../Server/src/tcp-ip/TcpIpServer.h"
#include "IDGenerator.h"

struct Lobby : ISerializable
{
    Json Serialize() override;
    void Deserialize(Json j) override;

    Lobby() : ID(IDGenerator::GenerateLobbyID())
        , PlayerCount(0)
        , MaxPlayerCount(2)
        , PlayerConnections()
    {}
    Lobby(int id, int playercount, int maxPlayercount)
        : ID(id)
        , PlayerCount(playercount)
        , MaxPlayerCount(maxPlayercount)
        , PlayerConnections()
    {
    }
    ~Lobby(){}

    void AddPlayerToLobby(ClientPtr playerConnection);

    int ID;
    int PlayerCount;
    int MaxPlayerCount;

    std::vector<ClientPtr> PlayerConnections;
};

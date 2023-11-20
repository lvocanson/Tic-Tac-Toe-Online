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
        , PlayerX()
        , PlayerO()
    {}
    ~Lobby(){}

    void AddPlayerToLobby(ClientPtr playerConnection);

    int ID;
    std::string PlayerX, PlayerO;
};

#pragma once
#include "Message.h"
#include "../game/Lobby.h"

template <>
struct Message<MsgType::LobbyList> : ISerializable
{
    Message() = default;
    Message(const Json& j)
        : LobbiesData()
    {
        LobbiesData.reserve(j["Lobbies"].size());

        for (auto& lobby : j["Lobbies"])
        {
            LobbiesData.push_back(LobbyData(lobby));
        }
    }
    ~Message() = default;

    Json Serialize() override
    {
        Json j;
        j["Type"] = MsgType::LobbyList;

        for (auto& lobby : LobbiesData)
        {
            j["Lobbies"].push_back(lobby.Serialize());
        }

        return j;
    }

    std::vector<LobbyData> LobbiesData;
};

template <>
struct Message<MsgType::GameStarted> : ISerializable
{
    Message() = default;
    Message(const Json& j)
        : StartPlayer(j["StartPlayer"]), PlayerX(j["PlayerX"]), PlayerO(j[PlayerO])
    {
    }
    ~Message() = default;

    Json Serialize() override
    {
        Json j;
        j["Type"] = MsgType::GameStarted;
        j["StartPlayer"] = StartPlayer;
        j["PlayerX"] = PlayerX;
        j["PlayerO"] = PlayerO;

        return j;
    }

    std::string StartPlayer, PlayerX, PlayerO;

};

template <>
struct Message<MsgType::AcceptMakeMove> : ISerializable
{
    Message() = default;
    Message(const Json& j)
        : LobbyId(j["ID"]), Cell(j["Cell"])
    {
    }
    ~Message() = default;

    Json Serialize() override
    {
        Json j;
        j["Type"] = MsgType::AcceptMakeMove;
        j["ID"] = LobbyId;
        j["Cell"] = Cell;
        return j;
    }

    unsigned int LobbyId;
    unsigned int Cell;
};

template <>
struct Message<MsgType::GameOver> : ISerializable
{
    Message() = default;
    Message(const Json& j)
        : Winner(j["Winner"])
    {
    }
    ~Message() = default;

    Json Serialize() override
    {
        Json j;
        j["Type"] = MsgType::GameOver;
        j["Winner"] = Winner;
        return j;
    }

    std::string Winner;
};

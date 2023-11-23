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
struct Message<MsgType::GameHistoryList> : ISerializable
{
    Message() = default;
    Message(const Json& j)
    {
        GameHistory.reserve(j["GameHistory"].size());

        for (auto& game : j["GameHistory"])
        {
            GameHistory.push_back(GameData(game));
        }
    }
    ~Message() = default;

    Json Serialize() override
    {
        Json j;
        j["Type"] = MsgType::GameHistoryList;

        for (auto& game : GameHistory)
        {
            j["GameHistory"].push_back(game.Serialize());
        }

        return j;
    }

    std::vector<GameData> GameHistory;
};

template <>
struct Message<MsgType::GameStarted> : ISerializable
{
    Message() = default;
    Message(const Json& j)
        : StartPlayer(j["StartPlayer"]), GameMode(j["GameMode"]), PlayerX(j["PlayerX"]), PlayerO(j["PlayerO"])
    {
    }
    ~Message() = default;

    Json Serialize() override
    {
        Json j;
        j["Type"] = MsgType::GameStarted;
        j["GameMode"] = GameMode;
        j["StartPlayer"] = StartPlayer;
        j["PlayerX"] = PlayerX;
        j["PlayerO"] = PlayerO;

        return j;
    }

    GameModeType GameMode;
    std::string StartPlayer, PlayerX, PlayerO;

};

template <>
struct Message<MsgType::AcceptMakeMove> : ISerializable
{
    Message() = default;
    Message(const Json& j)
        : Cell(j["Cell"].get<unsigned int>())
        , Piece(j["Piece"].get<TicTacToe::Piece>())
    {
    }
    ~Message() = default;

    Json Serialize() override
    {
        Json j;
        j["Type"] = MsgType::AcceptMakeMove;
        j["Cell"] = Cell;
        j["Piece"] = Piece;
        return j;
    }

    unsigned int LobbyId;
    unsigned int Cell;
    TicTacToe::Piece Piece;
};

template <>
struct Message<MsgType::GameOver> : ISerializable
{
    Message() = default;
    Message(const Json& j)
        : Winner(j["Winner"]), Piece(j["Piece"]), IsDraw(j["IsDraw"].get<bool>())
    {
    }
    ~Message() = default;

    Json Serialize() override
    {
        Json j;
        j["Type"] = MsgType::GameOver;
        j["Winner"] = Winner;
        j["Piece"] = Piece;
        j["IsDraw"] = IsDraw;
        return j;
    }

    bool IsDraw = false;
    std::string Winner;
    TicTacToe::Piece Piece;
};

#pragma once
#include "Message.h"
#include "../game/TicTacToe.h"

template <>
struct Message<MsgType::Login> : ISerializable
{
    Message() = default;
    Message(const Json& j)
    {
        Username = j["Username"].get<std::string>();
    }
    ~Message() = default;

    Json Serialize() override
    {
        Json j;
        j["Type"] = MsgType::Login;
        j["Username"] = Username;
        return j;
    }

    std::string Username;
};

template <>
struct Message<MsgType::JoinLobby> : ISerializable
{
    Message() = default;
    Message(const Json& j)
    {
        LobbyId = j["ID"].get<unsigned int>();
    }
    ~Message() = default;

    Json Serialize() override
    {
        Json j;
        j["Type"] = MsgType::JoinLobby;

        return j;
    }

    unsigned int LobbyId;
};

template <>
struct Message<MsgType::MakeMove> : ISerializable
{
    Message() = default;
    Message(const Json& j)
    {
        LobbyId = j["ID"].get<unsigned int>();
        Cell = j["Cell"].get<unsigned int>();
        Piece = j["Piece"].get<TicTacToe::Piece>();
    }
    ~Message() = default;

    Json Serialize() override
    {
        Json j;
        j["Type"] = MsgType::MakeMove;
        j["ID"] = LobbyId;
        j["Cell"] = Cell;
        j["Piece"] = Piece;
        return j;
    }

    unsigned int LobbyId;
    unsigned int Cell;
    TicTacToe::Piece Piece;
};

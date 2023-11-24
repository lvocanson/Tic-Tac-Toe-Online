#pragma once
#include "ISerializable.h"

enum class MsgType : unsigned int
{
    Unknown = 0,

    // Client -> Server

    Login,
    Disconnect,

    FetchGameHistoryList,
    FetchLobbyList,

    TryToJoinLobby,
    OnEnterLobby,
    LeaveLobby,


    MakeMove,

    // Server -> Client

    GameHistoryList,
    LobbyList,
    RefreshLobbyList,

    AcceptJoinLobby,
    RejectJoinLobby,
    OpponentLeftLobby,

    GameStarted,
    AcceptMakeMove,
    DeclineMakeMove,
    GameOver,
};

template <MsgType T = MsgType::Unknown>
struct Message : ISerializable
{
    Message() = default;

    static MsgType GetType(const Json& j)
    {
        if (j.contains("Type"))
            return j["Type"].get<MsgType>();
        return MsgType::Unknown;
    }

    Json Serialize() override
    {
        Json j;
        j["Type"] = T;
        return j;
    }
};

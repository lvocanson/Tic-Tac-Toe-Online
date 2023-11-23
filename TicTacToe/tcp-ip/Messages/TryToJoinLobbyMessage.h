#pragma once
#include "../ISerializable.h"

struct TryToJoinLobbyMessage : ISerializable
{
    TryToJoinLobbyMessage() = default;
    TryToJoinLobbyMessage(int ID) : ID(ID) { }

    Json Serialize() override;
    void Deserialize(const Json& j) override;

    int ID;
};


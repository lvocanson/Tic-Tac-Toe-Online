#pragma once
#include "../ISerializable.h"

struct JoinedLobbyMessage : ISerializable
{
    JoinedLobbyMessage() = default;
    JoinedLobbyMessage(int ID) : ID(ID) { }

    Json Serialize() override;
    void Deserialize(const Json& j) override;

    int ID;
};

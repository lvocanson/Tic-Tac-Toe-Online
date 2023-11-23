#pragma once
#include "../ISerializable.h"

struct JoinedLobbyMessage : ISerializable
{
    JoinedLobbyMessage() = default;
    JoinedLobbyMessage(int ID) : ID(ID) { }

    Json Serialize() override;
    void Deserialize(Json j) override;

    int ID;
};

#pragma once
#include "../ISerializable.h"

struct GetLobbyListNotification : ISerializable
{
    Json Serialize() override;
    void Deserialize(const Json& j) override {}
};


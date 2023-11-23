#pragma once
#include "../ISerializable.h"

struct RegisterPlayerMessage : ISerializable
{
    RegisterPlayerMessage() = default;
    RegisterPlayerMessage(const std::string& name) : Name(name) { }

    Json Serialize() override;
    void Deserialize(const Json& j) override;

    std::string Name;
};


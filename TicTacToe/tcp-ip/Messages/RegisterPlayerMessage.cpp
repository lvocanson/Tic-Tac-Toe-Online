#include "RegisterPlayerMessage.h"

Json RegisterPlayerMessage::Serialize()
{
    Json j;
    j["Type"] = "Login";
    j["UserName"] = Name;

    return j;
}

void RegisterPlayerMessage::Deserialize(Json j)
{
    Name = j["Name"];
}
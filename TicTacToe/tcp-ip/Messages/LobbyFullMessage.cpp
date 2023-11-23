#include "LobbyFullMessage.h"

Json LobbyFullMessage::Serialize()
{
    Json j;
    j["Type"] = "SetPlayerShape";
    j["PlayerX"] = PlayerX;
    j["PlayerO"] = PlayerO;
    j["Starter"] = rand() % 100 <= 50 ? PlayerO : PlayerX;

    return j;
}

void LobbyFullMessage::Deserialize(Json j)
{
    PlayerX = j["PlayerX"];
    PlayerO = j["PlayerO"];
    StartingPlayer = j["Starter"];
}
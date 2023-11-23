#include "GameFinishedMessage.h"

Json GameFinishedMessage::Serialize()
{
    Json j;
    j["Type"] = "GameFinished";
    j["ID"] = LobbyID;
    j["Winner"] = WinnerName;

    return j;
}

void GameFinishedMessage::Deserialize(const Json& j)
{
    LobbyID = j["ID"];
    WinnerName = j["Winner"];
}

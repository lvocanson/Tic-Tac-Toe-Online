#include "IsLobbyFullNotification.h"

Json IsLobbyFullNotification::Serialize()
{
    Json j;
    j["Type"] = "IsLobbyFull";
    j["LobbyID"] = LobbyID;

    return j;
}

void IsLobbyFullNotification::Deserialize(const Json& j)
{
    LobbyID = j["LobbyID"];
}

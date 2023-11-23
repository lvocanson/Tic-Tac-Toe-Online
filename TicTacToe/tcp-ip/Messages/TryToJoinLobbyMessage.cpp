#include "TryToJoinLobbyMessage.h"

Json TryToJoinLobbyMessage::Serialize()
{
    Json j;
    j["Type"] = "TryToJoinLobby";
    j["LobbyID"] = ID;

    return j;
}

void TryToJoinLobbyMessage::Deserialize(Json j)
{
    ID = j["LobbyID"];
}
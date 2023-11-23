#include "JoinedLobbyMessage.h"

Json JoinedLobbyMessage::Serialize()
{
    Json j;
    j["Type"] = "JoinedLobby";
    j["CurrentLobbyID"] = ID;

    return j;
}

void JoinedLobbyMessage::Deserialize(Json j)
{
    ID = j["CurrentLobbyID"];
}
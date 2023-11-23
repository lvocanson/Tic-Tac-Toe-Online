#include "PlayerMoveMessage.h"

Json PlayerMoveMessage::Serialize()
{
    Json j;
    j["Type"] = "OpponentMove";
    j["PlayerName"] = PlayerName;
    j["PlayerMove"] = Cell;
    j["ID"] = LobbyID;

    return j;
}

void PlayerMoveMessage::Deserialize(Json j)
{
    PlayerName = j["PlayerName"];
    Cell = j["PlayerMove"];
    LobbyID = j["ID"];
}

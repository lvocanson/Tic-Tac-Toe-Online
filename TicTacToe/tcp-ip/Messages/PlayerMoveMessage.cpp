#include "PlayerMoveMessage.h"

Json PlayerMoveMessage::Serialize()
{
    Json j;
    j["Type"] = "OpponentMove";
    j["PlayerName"] = PlayerName;
    j["PlayerMove"] = Cell;
    j["ID"] = LobbyID;
    j["Piece"] = Piece;
    

    return j;
}

void PlayerMoveMessage::Deserialize(const Json& j)
{
    PlayerName = j["PlayerName"];
    Cell = j["PlayerMove"];
    LobbyID = j["ID"];
    Piece = j["Piece"];
}

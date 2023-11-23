#include "PlayerMoveResponse.h"

Json PlayerMoveResponse::Serialize()
{
    Json j;
    j["Type"] = "PlayerMoveResponse";
    j["Cell"] = Cell;
    j["Piece"] = Piece;

    return j;
}

void PlayerMoveResponse::Deserialize(const Json& j)
{
    Cell = j["Cell"];
    Piece = j["Piece"];
}

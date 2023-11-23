#include "GameData.h"

GameData::GameData(const std::vector<PlayerMove>& allMoves, const std::string& playerX, const std::string& playerO)
{
    for (auto& move : allMoves)
    {
        AllMoves.emplace_back(move);
    }

    PlayerX = playerX;
    PlayerO = playerO;
    DateTime = std::format("{:%d-%m-%Y %H:%M:%OS}", std::chrono::system_clock::now());
}

GameData::GameData(const Json& j)
{
    for (auto& move : j["AllMoves"])
    {
        AllMoves.emplace_back(PlayerMove(move));
    }

    DateTime = j["DateTime"];
    PlayerO = j["PlayerO"];
    PlayerX = j["PlayerX"];
}

Json GameData::Serialize()
{
    Json j;

    for (auto& move : AllMoves)
    {
        j["AllMoves"].push_back(move.Serialize());
    }

    j["PlayerX"] = PlayerX;
    j["PlayerO"] = PlayerO;
    j["DateTime"] = DateTime;
    
    return j;
}

Json PlayerMove::Serialize()
{
    Json j;
    j["PlayerName"] = PlayerName;
    j["PlayerPiece"] = PlayerPiece;
    j["BoardCell"] = BoardCell;

    return j;
}

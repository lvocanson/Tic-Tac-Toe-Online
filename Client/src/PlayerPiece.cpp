#include "src/pch.h"
#include "PlayerPiece.h"

PlayerPiece::PlayerPiece(TicTacToe::Player* player)
{
    m_PlayerID = player->GetPlayerID();
}

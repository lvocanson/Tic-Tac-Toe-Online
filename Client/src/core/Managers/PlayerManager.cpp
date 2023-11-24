#include "PlayerManager.h"

#include "src/core/ClientApp.h"

PlayerManager::PlayerManager()
= default;

PlayerManager::~PlayerManager()
{
    Clear();
}

void PlayerManager::Init()
{
    m_IsPlayerTurn = false;
    m_CurrentPlayer = nullptr;
    m_OpponentPlayer = nullptr;
}

void PlayerManager::Clear()
{
    m_IsPlayerTurn = false;
    m_CurrentPlayer = nullptr;
    m_OpponentPlayer = nullptr;
}

void PlayerManager::SwitchPlayerTurn()
{
    m_IsPlayerTurn = !m_IsPlayerTurn;
}

void PlayerManager::CreateNewPlayer(const std::string& name, const sf::Color color, const TicTacToe::Piece piece)
{
    m_RegisteredPlayers.insert({ piece, Player(name, color, piece) });
    PlayerShapeRegistry::CreatePlayerShape(piece, color);
}

void PlayerManager::InitPlayerTurn(const std::string& starter, const TicTacToe::Piece& playerPiece)
{
    const std::string playerName = ClientApp::GetInstance().GetCurrentPlayer()->GetName();
    const TicTacToe::Piece opponentPiece = playerPiece == TicTacToe::Piece::X ? TicTacToe::Piece::O : TicTacToe::Piece::X;

    if (playerName == m_RegisteredPlayers.at(playerPiece).GetName())
    {
        m_CurrentPlayer = &m_RegisteredPlayers.at(playerPiece);
        m_OpponentPlayer = &m_RegisteredPlayers.at(opponentPiece);
    }
    else if (playerName == m_RegisteredPlayers.at(opponentPiece).GetName())
    {
        m_CurrentPlayer = &m_RegisteredPlayers.at(opponentPiece);
        m_OpponentPlayer = &m_RegisteredPlayers.at(playerPiece);
    }
    else
    {
        DebugLog("Invalid starter name");
    }

    m_IsPlayerTurn = starter == playerName;
}

Player& PlayerManager::GetPlayerByPiece(TicTacToe::Piece piece)
{
    if (m_RegisteredPlayers.contains(piece))
        return m_RegisteredPlayers[piece];

    DebugLog("Player not found");
}

Player& PlayerManager::GetOpponentPlayerByPiece(TicTacToe::Piece piece)
{
    const TicTacToe::Piece opponentPiece = piece == TicTacToe::Piece::X ? TicTacToe::Piece::O : TicTacToe::Piece::X;

    if (m_RegisteredPlayers.contains(opponentPiece))
        return m_RegisteredPlayers[opponentPiece];
}

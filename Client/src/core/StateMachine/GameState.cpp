#include "GameState.h"
#include "TicTacToe.h"

#include "src/core/Window.h"
#include "src/core/PlayerPiece.h"

#include <SFML/System/Time.hpp>

using namespace TicTacToe;

GameState::GameState(StateMachine* stateMachine, Window* m_Window)
	: State(stateMachine)
	, m_Window(m_Window)
{
}

void GameState::OnEnter()
{
    m_Board.Init();

    m_PlayerOne.SetName("Player One");
    m_PlayerTwo.SetName("Player Two");

    DrawBoard();
}

void GameState::OnUpdate(float dt)
{
    if (m_PlayerTurnTimer > sf::Time::Zero)
    {
        m_PlayerTurnTimer -= sf::seconds(dt);
        return;
    }

    CheckIfMouseHoverBoard();
}

void GameState::OnExit()
{
    ClearBoard();
}

void GameState::DrawBoard()
{
    const float pieceSize = m_Board.GetPieceSize();
    const size_t width = m_Board.GetWidth();
    const size_t height = m_Board.GetHeight();
    const sf::Vector2f center = m_Window->GetCenter();

    // Draw the board - temp
    for (unsigned int i = 0; i < m_Board.GetTotalSize(); ++i)
    {
        auto* square = new sf::RectangleShape(sf::Vector2f(pieceSize, pieceSize));
        square->setFillColor(sf::Color::Color(51, 56, 63));
        square->setOutlineColor(sf::Color::Color(0, 189, 156));
        square->setOutlineThickness(OUTLINE_THICKNESS);
        square->setPosition(center.x - (width * pieceSize * 0.5f) + (i % width) * pieceSize + OUTLINE_THICKNESS * (i % width),
            center.y - (height * pieceSize * 0.5f) + (i / height) * pieceSize + OUTLINE_THICKNESS * (i / height));

        m_Window->RegisterDrawable(square);
        m_Board.GetGraphicPiece(i).SetShape(square);
        m_Board.GetGraphicPiece(i).SetPosition(square->getPosition());
    }
}

void GameState::CheckIfMouseHoverBoard()
{
    for (unsigned int i = 0; i < m_Board.GetTotalSize(); i++)
    {
        if (m_Board[i] != EMPTY_PIECE) continue;

        if (IsMouseHoverPiece(i))
        {
            if (m_Window->IsMouseButtonPressed(sf::Mouse::Left))
            {
                PlacePlayerPieceOnBoard(i);

                const int winnerID = m_Board.IsThereAWinner();
                if (winnerID != EMPTY_PIECE)
                {
                    std::cout << "Player " << winnerID << " won!\n";
                }

                if (m_Board.IsFull() || winnerID != EMPTY_PIECE)
                    ClearBoard();

                SwitchPlayerTurn();
            }
        }
    }
}

void GameState::PlacePlayerPieceOnBoard(unsigned int i)
{
    m_Board[i] = (m_IsPlayerOneTurn ? m_PlayerOne.GetPlayerID() : m_PlayerTwo.GetPlayerID());

    auto pos = sf::Vector2f(m_Board.GetGraphicPiece(i).GetPosition());

    // Center the piece
    pos.x += static_cast<float>(m_Board.GetPieceSize()) * 0.5f;
    pos.y += static_cast<float>(m_Board.GetPieceSize()) * 0.5f;

    if (m_IsPlayerOneTurn)
    {
        auto* piece = new PlayerCircleShape(&m_PlayerOne);
        piece->setPosition(pos);
        m_Window->RegisterDrawable(piece);
        m_GamePieces.push_back(piece);
    }
    else
    {
        auto* piece = new PlayerCrossShape(&m_PlayerTwo);
        piece->setPosition(pos);
        m_Window->RegisterDrawable(piece);
        m_GamePieces.push_back(piece);
    }
}

void GameState::ClearBoard()
{
    for (auto& piece : m_GamePieces)
    {
        m_Window->UnregisterDrawable(piece);
        RELEASE(piece);
    }

    m_GamePieces.clear();
    m_Board.SetEmpty();
}

void GameState::SwitchPlayerTurn()
{
    m_IsPlayerOneTurn = !m_IsPlayerOneTurn;
    m_PlayerTurnTimer = sf::seconds(PLAYER_TURN_DELAY);
}

bool GameState::IsMouseHoverPiece(unsigned int i)
{
    const sf::Vector2f mousePos = static_cast<sf::Vector2f>(m_Window->GetMousePosition());
    const float size = m_Board.GetPieceSize();
    const sf::Vector2f piecePosition = m_Board.GetGraphicPiece(i).GetPosition();

    return  mousePos.x > piecePosition.x &&
        mousePos.x < piecePosition.x + size &&
        mousePos.y > piecePosition.y &&
        mousePos.y < piecePosition.y + size;
}
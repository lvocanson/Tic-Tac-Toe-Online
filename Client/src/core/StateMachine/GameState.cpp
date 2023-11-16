#include "GameState.h"
#include "TicTacToe.h"

#include "src/core/Window.h"
#include "src/core/PlayerPieceShape.h"
#include <SFML/System/Time.hpp>

using namespace TicTacToe;

GameState::GameState(StateMachine* stateMachine, Window* m_Window)
	: State(stateMachine)
	, m_Window(m_Window)
{
    m_StateMachine = stateMachine;
    m_PlayerManager.CreateNewPlayer("Player One", sf::Color(250, 92, 12));
    m_PlayerManager.CreateNewPlayer("Player Two", sf::Color(255, 194, 0));

    m_Board.Init();
    m_ScoreManager.Init();
    m_PlayerManager.Init();

    for (const auto& player : m_PlayerManager.GetAllPlayers())
    {
        m_ScoreManager.CreateScoreForPlayer(player->GetData(), m_Window);
    }
}

GameState::~GameState()
{
    NULLPTR(m_Window);
}

void GameState::OnEnter()
{
    m_GameStateUI = new GameStateUI(m_Window);
    m_GameStateUI->Init();
    m_GameStateUI->InitPlayerScores(m_PlayerManager.GetAllPlayers());

    m_ReturnButton = new ButtonComponent(sf::Vector2f(100, 500), sf::Vector2f(200, 100), sf::Color::Red, sf::Color::White, "Return", sf::Color::White, 30, TextAlignment::Center);
    m_ReturnButton->SetOnClickCallback([this]() {
        m_StateMachine->SwitchState("MenuState");
      });

    m_Window->RegisterDrawable(m_ReturnButton);
    DrawBoard();

}

void GameState::OnUpdate(float dt)
{
    if (m_PlayerTurnTimer > sf::Time::Zero)
    {
        m_PlayerTurnTimer -= sf::seconds(dt);
        return;
    }

    // TODO : REWORK THIS SHIT FUCKEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEER
    m_ReturnButton->Update(m_Window);

    CheckIfMouseHoverBoard();
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
            if (Window::IsMouseButtonPressed(sf::Mouse::Left))
            {
                m_GameStateUI->UpdateGameStateText("");

                PlacePlayerPieceOnBoard(i);

                const PieceID winnerID = m_Board.IsThereAWinner();
                if (winnerID != EMPTY_PIECE)
                {
                    std::cout << "Player " << winnerID << " won!\n";

                    Player* winner = PlayerManager::GetCurrentPlayer();

                    m_ScoreManager.SaveGame(winner->GetData());
                    m_ScoreManager.AddScoreToPlayer(*winner->GetData());

                    m_GameStateUI->UpdatePlayerScore(*winner->GetData(), m_ScoreManager.GetPlayerScore(winner->GetPlayerID()));
                    m_GameStateUI->UpdateGameStateText(winner->GetName() + " won!");

                    ClearBoard();
                }
                else if (m_Board.IsFull())
                {
                    m_GameStateUI->UpdateGameStateText("It's a draw!");
                    ClearBoard();
                }

                SwitchPlayerTurn();
            }
        }
    }
}

void GameState::PlacePlayerPieceOnBoard(unsigned int cell)
{
    const Player* currentPlayer = PlayerManager::GetCurrentPlayer();

    // Set piece id in board
    m_Board[cell] = currentPlayer->GetPlayerID();

    SetGraphicalPiece(cell, currentPlayer);

    m_ScoreManager.AddPlayerMove(currentPlayer->GetPlayerID(), cell);
}

void GameState::SetGraphicalPiece(unsigned cell, const Player* currentPlayer)
{
    auto pos = sf::Vector2f(m_Board.GetGraphicPiece(cell).GetPosition());

    // Center the piece
    pos.x += m_Board.GetPieceSize() * 0.5f;
    pos.y += m_Board.GetPieceSize() * 0.5f;

    // TODO: rework this shit
    if (m_PlayerManager.IsPlayerOneTurn())
    {
        auto piece = new PlayerCircleShape(currentPlayer);
        piece->setPosition(pos);
        m_Window->RegisterDrawable(piece);
        m_GamePieces.push_back(piece);
    }
    else
    {
        auto* piece = new PlayerCrossShape(currentPlayer);
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
    m_PlayerManager.SwitchPlayerTurn();

    m_PlayerTurnTimer = sf::seconds(PLAYER_TURN_DELAY);
    m_GameStateUI->UpdatePlayerTurnText(*PlayerManager::GetCurrentPlayer()->GetData());

    // TODO : Change color based on player turn
    /*if (m_PlayerManager.IsPlayerOneTurn())
        m_PlayerTurnText->setFillColor(sf::Color::Color(250, 92, 12));
    else
        m_PlayerTurnText->setFillColor(sf::Color::Color(255, 194, 0));*/
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

void GameState::OnExit()
{
    ClearBoard();
    RELEASE(m_GameStateUI);

    m_Window->ClearAllDrawables();
}

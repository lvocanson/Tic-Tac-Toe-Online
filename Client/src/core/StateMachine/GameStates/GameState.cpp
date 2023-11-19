#include "GameState.h"
#include "TicTacToe.h"

#include "src/core/Window.h"
#include "src/core/PlayerPieceShape.h"
#include "src/core/ClientApp.h"

#include <SFML/System/Time.hpp>

using namespace TicTacToe;
using json = nlohmann::json;


GameState::GameState(StateMachine* stateMachine, Window* m_Window)
    : State(stateMachine)
    , m_Window(m_Window)
    , m_GameStateUI(nullptr)
    , m_ReturnButton(nullptr)
{
    m_StateMachine = stateMachine;
    m_PlayerManager.CreateNewPlayer("Player One", sf::Color(250, 92, 12), Square);
    m_PlayerManager.CreateNewPlayer("Player Two", sf::Color(255, 194, 0), Circle);

    m_Board.Init(ClientApp::GetGameSettings().TotalColumn, ClientApp::GetGameSettings().TotalRow);
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
    m_MaxPlayerTurnTime = ClientApp::GetGameSettings().PlayerMoveLimitTime;
    m_PlayerTurnTime = m_MaxPlayerTurnTime;

    m_GameStateUI = new GameStateUI(m_Window);
    m_GameStateUI->Init();
    m_GameStateUI->InitPlayerScores(m_PlayerManager.GetAllPlayers());
    m_GameStateUI->InitProgressBar(m_MaxPlayerTurnTime);

    m_ReturnButton = new ButtonComponent(sf::Vector2f(100, 500), sf::Vector2f(200, 100), sf::Color::Red, sf::Color::White);
    m_ReturnButton->SetButtonText("Return", sf::Color::White, 30, TextAlignment::Center);
    m_ReturnButton->SetOnClickCallback([this]() {
        m_StateMachine->SwitchState("MenuState");
    });

    m_Window->RegisterDrawable(m_ReturnButton);
    DrawBoard();
}


void GameState::OnUpdate(float dt)
{
    m_ReturnButton->Update();

    CheckIfMouseHoverBoard();

    UpdatePlayerTimer(dt);
    CheckIfTimerIsUp();
}

void GameState::UpdatePlayerTimer(float dt)
{
    m_PlayerTurnTime -= dt;
    m_GameStateUI->UpdateProgressBar(m_PlayerTurnTime);
}

void GameState::CheckIfTimerIsUp()
{
    if (m_PlayerTurnTime <= 0.0f)
    {
        m_GameStateUI->UpdateGameStateText("Time's up!");

        PlacePlayerPieceOnBoard(m_Board.GetRandomEmptyCell());
        WinCheck();
        SwitchPlayerTurn();
    }
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
            if (InputHandler::IsMouseButtonPressed(sf::Mouse::Left))
            {
                m_GameStateUI->UpdateGameStateText("");

                PlacePlayerPieceOnBoard(i);
                WinCheck();
                SwitchPlayerTurn();
            }
        }
    }
}

void GameState::PlacePlayerPieceOnBoard(unsigned int cell)
{
    const Player* currentPlayer = PlayerManager::GetCurrentPlayer();

    int row = cell / (int)m_Board.GetWidth();
    int col = cell % (int)m_Board.GetWidth();
    std::string playerID = std::to_string(m_Board[cell]);
    json j;
    j["row"] = row;
    j["col"] = col;
    j["playerID"] = playerID;

    ClientApp::GetInstance().Send(j.dump());

    auto pos = sf::Vector2f(m_Board.GetGraphicPiece(cell).GetPosition());
    // Set piece id in board
    m_Board[cell] = currentPlayer->GetPlayerID();

    InstanciateNewPlayerShape(currentPlayer, cell);

    m_ScoreManager.AddPlayerMove(currentPlayer->GetPlayerID(), cell);
}

void GameState::InstanciateNewPlayerShape(const Player* currentPlayer, unsigned int cell)
{
    auto pos = sf::Vector2f(m_Board.GetGraphicPiece(cell).GetPosition());

    // Center the piece
    pos.x += m_Board.GetPieceSize() * 0.5f;
    pos.y += m_Board.GetPieceSize() * 0.5f;

    auto playerPieceShape = new PlayerPieceShape(currentPlayer->GetPlayerID(), pos);
    m_GamePieces.push_back(playerPieceShape);
    m_Window->RegisterDrawable(playerPieceShape);
}

void GameState::WinCheck()
{
    const PieceID winnerID = m_Board.IsThereAWinner();
    if (winnerID != EMPTY_PIECE)
    {
        std::cout << "Player " << winnerID << " won!\n";

        Player* winner = PlayerManager::GetCurrentPlayer();

        m_ScoreManager.SaveGame(winner->GetData());
        m_ScoreManager.AddScoreToPlayer(winner->GetPlayerID());

        m_GameStateUI->UpdatePlayerScore(*winner->GetData(), m_ScoreManager.GetPlayerScore(winner->GetPlayerID()));
        m_GameStateUI->UpdateGameStateText(winner->GetName() + " won!");

        ClearBoard();
    }
    else if (m_Board.IsFull())
    {
        m_GameStateUI->UpdateGameStateText("It's a draw!");
        ClearBoard();
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
    m_GameStateUI->UpdatePlayerTurnText(*PlayerManager::GetCurrentPlayer()->GetData());

    m_PlayerTurnTime = m_MaxPlayerTurnTime;
    m_GameStateUI->UpdateProgressBar(m_PlayerTurnTime);
}

bool GameState::IsMouseHoverPiece(unsigned int i)
{
    const sf::Vector2f mousePos = static_cast<sf::Vector2f>(InputHandler::GetMousePosition());
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

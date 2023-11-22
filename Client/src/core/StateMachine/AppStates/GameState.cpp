#include "GameState.h"
#include "TicTacToe.h"

#include "src/core/Window.h"
#include "src/core/PlayerPieceShape.h"
#include "src/core/ClientApp.h"

using namespace TicTacToe;
using json = nlohmann::json;

GameState::GameState(StateMachine *stateMachine, Window *m_Window)
    : State(stateMachine), m_Window(m_Window), m_ReturnButton(nullptr), m_GameStateUI(nullptr), m_IsTimerOn(false)
{
    m_PlayerManager.CreateNewPlayer("Player One", sf::Color(250, 92, 12), Square);
    m_PlayerManager.CreateNewPlayer("Player Two", sf::Color(255, 194, 0), Circle);

    m_Board.Init(ClientApp::GetGameSettings().GetTotalColumn(), ClientApp::GetGameSettings().GetTotalRow(), m_Window);
    m_PlayerManager.Init();
}

GameState::~GameState()
{
    NULLPTR(m_Window);
}

void GameState::OnEnter()
{
    m_MaxPlayerTurnTime = ClientApp::GetGameSettings().GetPlayerMoveLimitTime();
    m_IsTimerOn = ClientApp::GetGameSettings().IsTimerOn();
    m_PlayerTurnTime = m_MaxPlayerTurnTime;

    m_GameStateUI = new GameStateUI(m_Window);
    m_GameStateUI->Init();
    m_GameStateUI->InitPlayerScores(m_PlayerManager.GetAllPlayers());

    if (m_IsTimerOn)
    {
        m_GameStateUI->InitProgressBar(m_MaxPlayerTurnTime);
    }

    m_ReturnButton = new ButtonComponent(sf::Vector2f(100, 500), sf::Vector2f(200, 100), sf::Color::Red, sf::Color::White);
    m_ReturnButton->SetButtonText("Return", sf::Color::White, 30, TextAlignment::Center);
    m_ReturnButton->SetOnClickCallback([this]()
                                       { m_StateMachine->SwitchState("MenuState"); });

    m_Window->RegisterDrawable(m_ReturnButton);

    m_ScoreManager.Init();
    m_ScoreManager.InitPlayerScores(m_PlayerManager.GetAllPlayers());

    m_Board.DrawBoard();
}

void GameState::OnUpdate(float dt)
{
    m_ReturnButton->Update();

    CheckIfMouseHoverBoard();

    if (m_IsTimerOn)
    {
        UpdatePlayerTimer(dt);
        CheckIfTimerIsUp();
    }
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

void GameState::CheckIfMouseHoverBoard()
{
    for (unsigned int i = 0; i < m_Board.GetTotalSize(); i++)
    {
        if (m_Board[i] != EMPTY_PIECE)
            continue;

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
    Player *currentPlayer = PlayerManager::GetCurrentPlayer();

    // Set piece id in board
    m_Board[cell] = currentPlayer->GetPlayerID();

    m_Board.InstanciateNewPlayerShape(currentPlayer->GetPlayerID(), cell);

    m_ScoreManager.AddPlayerMove(*currentPlayer->GetData(), cell);

    SendPlacedPieceToServer(cell);
}

void GameState::WinCheck()
{
    const PieceID winnerID = m_Board.IsThereAWinner();
    if (winnerID != EMPTY_PIECE)
    {
        std::cout << "Player " << winnerID << " won!\n";

        Player *winner = PlayerManager::GetCurrentPlayer();

        ClientApp::GetHistoryManager()->SaveGame(winner->GetData(), m_ScoreManager.GetCurrentGame());
        m_ScoreManager.AddScoreToPlayer(winner->GetPlayerID());
        m_ScoreManager.CreateNewGameHistory();

        m_GameStateUI->UpdatePlayerScore(*winner->GetData(), m_ScoreManager.GetPlayerScore(winner->GetPlayerID()));
        m_GameStateUI->UpdateGameStateText(winner->GetName() + " won!");

        ClearBoard();
    }
    else if (m_Board.IsFull())
    {
        m_GameStateUI->UpdateGameStateText("It's a draw!");
        m_ScoreManager.ResetCurrentGame();
        ClearBoard();
    }
}

void GameState::ClearBoard()
{
    m_Board.SetEmpty();
}

void GameState::SwitchPlayerTurn()
{
    m_PlayerManager.SwitchPlayerTurn();
    m_GameStateUI->UpdatePlayerTurnText(*PlayerManager::GetCurrentPlayer()->GetData());

    if (m_IsTimerOn)
    {
        m_PlayerTurnTime = m_MaxPlayerTurnTime;
        m_GameStateUI->UpdateProgressBar(m_PlayerTurnTime);
    }
}

void GameState::SendPlacedPieceToServer(unsigned int cell)
{
    int row = cell / (int)m_Board.GetWidth();
    int col = cell % (int)m_Board.GetWidth();
    std::string playerID = std::to_string(m_Board[cell]);

    json j;
    j["row"] = row;
    j["col"] = col;
    j["playerID"] = playerID;

    ClientApp::GetInstance().Send(j.dump());
}

bool GameState::IsMouseHoverPiece(unsigned int i)
{
    const sf::Vector2f mousePos = static_cast<sf::Vector2f>(InputHandler::GetMousePosition());
    const float size = m_Board.GetPieceSize();
    const sf::Vector2f piecePosition = m_Board.GetGraphicPiece(i).GetPosition();

    return mousePos.x > piecePosition.x &&
           mousePos.x < piecePosition.x + size &&
           mousePos.y > piecePosition.y &&
           mousePos.y < piecePosition.y + size;
}

void GameState::OnExit()
{
    ClearBoard();
    RELEASE(m_GameStateUI);
    m_ScoreManager.Clear();

    m_Window->ClearAllDrawables();
}

void GameState::OnReceiveData(const Json &serializeData)
{
    std::string playerID = serializeData["playerID"];
    int row = serializeData["row"];
    int col = serializeData["col"];
    // Use player manager to to set player
}

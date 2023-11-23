#include "GameState.h"

#include "tcp-ip/Messages/GameFinishedMessage.h"
#include "tcp-ip/Notifications/IsLobbyFullNotification.h"
#include "tcp-ip/Messages/PlayerMoveMessage.h"
#include "src/core/Window.h"
#include "src/core/ClientApp.h"
#include "tcp-ip/Messages/LobbyFullMessage.h"

using namespace TicTacToe;
using json = nlohmann::json;

GameState::GameState(StateMachine* stateMachine, Window* m_Window)
    : State(stateMachine)
    , m_Window(m_Window)
{
    m_GameStateUI = new GameStateUI(m_Window);

    m_Board.Init(ClientApp::GetGameSettings().GetTotalColumn(), ClientApp::GetGameSettings().GetTotalRow(), m_Window);
    m_PlayerManager.Init();
}

GameState::~GameState()
{
    NULLPTR(m_Window);
    RELEASE(m_GameStateUI);
}

void GameState::OnEnter()
{
    IsServerLobbyFull();

    m_ScoreManager.Init();
    m_GameStateUI->Init();

    m_MaxPlayerTurnTime = ClientApp::GetGameSettings().GetPlayerMoveLimitTime();
    m_IsTimerOn = ClientApp::GetGameSettings().IsTimerOn();
    m_PlayerTurnTime = m_MaxPlayerTurnTime;

    if (m_IsTimerOn)
    {
        m_GameStateUI->InitProgressBar(m_MaxPlayerTurnTime);
    }

    m_ReturnButton = new ButtonComponent(sf::Vector2f(100, 500), sf::Vector2f(200, 100), sf::Color::Red);
    m_ReturnButton->SetButtonText("Leave game", sf::Color::White, 30, TextAlignment::Center);
    m_ReturnButton->SetOnClickCallback([this]()
    { 
        Json j;
        j["Type"] = "LeaveLobby";
        j["ID"] = m_LobbyID;
        ClientConnectionHandler::GetInstance().SendDataToServer(j.dump());
        m_StateMachine->SwitchState("MenuState"); 
    });

    m_Window->RegisterDrawable(m_ReturnButton);

    m_Board.DrawBoard();
}

void GameState::OnUpdate(float dt)
{
    if (m_ReturnButton)
    {
        m_ReturnButton->Update(dt);
    }

    if (m_IsPlayerTurn && m_IsGameStarted)
    {
        CheckIfMouseHoverBoard();

        if (m_IsTimerOn)
        {
            UpdatePlayerTimer(dt);
            CheckIfTimerIsUp();
        }
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
    for (unsigned int cell = 0; cell < m_Board.GetTotalSize(); cell++)
    {
        if (m_Board[cell] != EMPTY_PIECE)
            continue;

        if (IsMouseHoverPiece(cell))
        {
            if (InputHandler::IsMouseButtonPressed(sf::Mouse::Left))
            {
                m_GameStateUI->UpdateGameStateText("");

                PlacePlayerPieceOnBoard(cell);
                SendPlacedPieceToServer(cell);
                WinCheck();
                SwitchPlayerTurn();
            }
        }
    }
}

void GameState::PlacePlayerPieceOnBoard(unsigned int cell)
{
    Player* currentPlayer = PlayerManager::GetCurrentPlayer();

    // Set piece id in board
    m_Board[cell] = currentPlayer->GetPlayerID();

    m_Board.InstanciateNewPlayerShape(currentPlayer->GetPlayerID(), currentPlayer->GetShapeType(), cell);

    m_ScoreManager.AddPlayerMove(*currentPlayer->GetData(), cell);
}

void GameState::WinCheck()
{
    const PieceID winnerID = m_Board.IsThereAWinner();
    if (winnerID != EMPTY_PIECE)
    {
        std::cout << "Player " << winnerID << " won!\n";

        Player* winner = PlayerManager::GetCurrentPlayer();

        ClientApp::GetHistoryManager()->SaveGame(winner->GetData(), m_ScoreManager.GetCurrentGame());
        m_ScoreManager.AddScoreToPlayer(winner->GetPlayerID());
        m_ScoreManager.CreateNewGameHistory();

        m_GameStateUI->UpdatePlayerScore(*winner->GetData(), m_ScoreManager.GetPlayerScore(winner->GetPlayerID()));
        m_GameStateUI->UpdateGameStateText(winner->GetName() + " won!");

        SendGameFinishedToServer(winner->GetName());
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
    m_IsPlayerTurn = !m_IsPlayerTurn;
    m_PlayerManager.SwitchPlayerTurn();
    m_GameStateUI->UpdatePlayerTurnText(*PlayerManager::GetOpponentPlayer()->GetData());

    if (m_IsTimerOn)
    {
        m_PlayerTurnTime = m_MaxPlayerTurnTime;
        m_GameStateUI->UpdateProgressBar(m_PlayerTurnTime);
    }
}

void GameState::SendPlacedPieceToServer(unsigned int cell)
{
    PlayerMoveMessage message(ClientApp::GetInstance().GetCurrentPlayer()->GetName(), cell, m_LobbyID);
    ClientConnectionHandler::GetInstance().SendDataToServer(message.Serialize().dump());
}

void GameState::SendGameFinishedToServer(const std::string& winnerName)
{
    GameFinishedMessage message(winnerName, m_LobbyID);
    ClientConnectionHandler::GetInstance().SendDataToServer(message.Serialize().dump());
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
    m_ScoreManager.Clear();

    m_Window->ClearAllDrawables();
}

void GameState::OnReceiveData(const Json& serializeData)
{
    if (serializeData["Type"] == "OpponentMove")
    {
        PlacePlayerPieceOnBoard(serializeData["PlayerMove"]);
        SwitchPlayerTurn();
    }
    else if (serializeData["Type"] == "SetPlayerShape")
    {
        LobbyFullMessage message;
        message.Deserialize(serializeData);

        m_PlayerManager.CreateNewPlayer(message.PlayerX, sf::Color(250, 92, 12), Square);
        m_PlayerManager.CreateNewPlayer(message.PlayerO, sf::Color(255, 194, 0), Circle);

        m_IsPlayerTurn = message.StartingPlayer == ClientApp::GetInstance().GetCurrentPlayer()->GetName();
        
        m_GameStateUI->UpdateGameStateText("It's " + message.StartingPlayer + " to start the game!");

        StartGame();
    }
    else if (serializeData["Type"] == "GameFinished")
    {
        GameFinishedMessage message;
        message.Deserialize(serializeData);

        m_GameStateUI->UpdateGameStateText(message.WinnerName + " won!");
        ClearBoard();
    }
}

void GameState::IsServerLobbyFull()
{
    IsLobbyFullNotification notification(m_LobbyID);
    ClientConnectionHandler::GetInstance().SendDataToServer(notification.Serialize().dump());
}

void GameState::StartGame()
{
    m_IsGameStarted = true;
    m_ScoreManager.InitPlayerScores(m_PlayerManager.GetAllPlayers());
}

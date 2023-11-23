#include "GameState.h"

#include "tcp-ip/Messages/GameFinishedMessage.h"
#include "tcp-ip/Notifications/IsLobbyFullNotification.h"
#include "tcp-ip/Messages/PlayerMoveMessage.h"
#include "src/core/Window.h"
#include "src/core/ClientApp.h"
#include "tcp-ip/Messages/LobbyFullMessage.h"
#include <PlayerMoveResponse.h>

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
    m_ReturnButton->SetButtonText("Return", sf::Color::White, 30, TextAlignment::Center);
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

        //PlacePlayerPieceOnBoard(m_Board.GetRandomEmptyCell());
    }
}

void GameState::CheckIfMouseHoverBoard()
{
    for (unsigned int cell = 0; cell < m_Board.GetTotalSize(); cell++)
    {
        if (m_Board[cell] != Piece::Empty)
            continue;

        if (IsMouseHoverPiece(cell))
        {
            if (InputHandler::IsMouseButtonPressed(sf::Mouse::Left))
            {
                m_GameStateUI->UpdateGameStateText("Waiting server");

                SendPlacedPieceToServer(cell);
            }
        }
    }
}

void GameState::PlacePlayerPieceOnBoard(unsigned int cell, TicTacToe::Piece piece)
{
    m_Board.InstanciateNewPlayerShape(piece, cell);
    //m_ScoreManager.AddPlayerMove(PlayerManager::GetPlayer(), cell);
}

void GameState::ClearBoard()
{
    m_Board.SetEmpty();
}

void GameState::SwitchPlayerTurn()
{
    m_IsPlayerTurn = !m_IsPlayerTurn;
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
    PlayerMoveMessage message(ClientApp::GetInstance().GetCurrentPlayer()->GetName(), cell, m_LobbyID, PlayerManager::GetCurrentPlayer()->GetData()->Piece);
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
    if (serializeData["Type"] == "PlayerMoveResponse")
    {
        PlayerMoveResponse moveResponse;
        moveResponse.Deserialize(serializeData);

        PlacePlayerPieceOnBoard(moveResponse.Cell, moveResponse.Piece);
        SwitchPlayerTurn();
    }
    else if (serializeData["Type"] == "SetPlayerShape")
    {
        LobbyFullMessage message;
        message.Deserialize(serializeData);

        m_PlayerManager.CreateNewPlayer(message.PlayerX, sf::Color(250, 92, 12), Piece::X);
        m_PlayerManager.CreateNewPlayer(message.PlayerO, sf::Color(255, 194, 0), Piece::O);

        m_IsPlayerTurn = message.StartingPlayer == ClientApp::GetInstance().GetCurrentPlayer()->GetName();

        StartGame();
    }
    else if (serializeData["Type"] == "GameFinished")
    {
        GameFinishedMessage message;
        message.Deserialize(serializeData);

        //ClientApp::GetHistoryManager()->SaveGame(winner->GetData(), m_ScoreManager.GetCurrentGame());

        m_ScoreManager.AddScoreToPlayer(message.Piece);
        m_ScoreManager.CreateNewGameHistory();

        m_GameStateUI->UpdatePlayerScore(message.Piece, message.WinnerName, m_ScoreManager.GetPlayerScore(message.Piece));
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
   // m_GameStateUI->InitPlayerScores(m_PlayerManager.GetAllPlayers());
}

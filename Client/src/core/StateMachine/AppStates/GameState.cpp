#include "GameState.h"
#include "src/core/Window.h"
#include "src/core/ClientApp.h"
#include "tcp-ip/ClientMessages.h"
#include "tcp-ip/ServerMessages.h"

using namespace TicTacToe;
using json = nlohmann::json;

GameState::GameState(StateMachine* stateMachine, Window* m_Window)
    : State(stateMachine)
    , m_Window(m_Window)
{
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
    m_GameStateUI = new GameStateUI(m_Window);

    m_IsTimerOn = false;

    m_ScoreManager.Init();
    m_GameStateUI->Init();
    m_GameStateUI->SetLobbyIDText(m_LobbyID);
    m_GameStateUI->SetUserName(ClientApp::GetInstance().GetCurrentPlayer()->GetName());

    m_ReturnButton = new ButtonComponent(sf::Vector2f(100, 500), sf::Vector2f(200, 100), sf::Color::Red);
    m_ReturnButton->SetButtonText("Leave game", sf::Color::White, 30, TextAlignment::Center);
    m_ReturnButton->SetOnClickCallback([this]()
    { 
        Message<MsgType::LeaveLobby> message;
        message.LobbyId = m_LobbyID;
        message.PlayerName = ClientApp::GetInstance().GetCurrentPlayer()->GetName();
        ClientConnectionHandler::GetInstance().SendDataToServer(message);

        m_StateMachine->SwitchState("MenuState"); 
    });

    m_Window->RegisterDrawable(m_ReturnButton);

    m_Board.DrawBoard();

    Message<MsgType::OnEnterLobby> message;
    message.LobbyId = m_LobbyID;
    ClientConnectionHandler::GetInstance().SendDataToServer(message);
}

void GameState::OnUpdate(float dt)
{
    if (m_ReturnButton)
    {
        m_ReturnButton->Update(dt);
    }

    if (!m_IsGameStarted) return;

    if (m_NeedToCleanBoard)
    {
        ClearBoard();
    }

    if (!m_WaitingServerResponse)
    {
        if (m_IsTimerOn)
        {
            UpdatePlayerTimer(dt);
            CheckIfTimerIsUp();
        }

        if (m_IsPlayerTurn)
            CheckIfMouseHoverBoard();
    }
}

void GameState::UpdatePlayerTimer(float dt)
{
    m_PlayerTurnTime -= dt;
    m_GameStateUI->UpdateProgressBar(m_PlayerTurnTime);
}

void GameState::CheckIfTimerIsUp() const
{
    if (m_PlayerTurnTime <= 0.0f)
    {
        m_GameStateUI->UpdateGameStateText("Time's up!");
        SendPlacedPieceToServer(m_Board.GetRandomEmptyCell());
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

void GameState::ClearBoard()
{
    m_NeedToCleanBoard = false;
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

void GameState::SendPlacedPieceToServer(unsigned int cell) const
{
    Message<MsgType::MakeMove> message;
    message.Cell = cell;
    message.LobbyId = m_LobbyID;
    message.Piece = PlayerManager::GetCurrentPlayer()->GetPiece();

    ClientConnectionHandler::GetInstance().SendDataToServer(message);
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

    RELEASE(m_GameStateUI)

    m_Window->ClearAllDrawables();
}

void GameState::OnReceiveData(const Json& serializeData)
{
    const auto type = Message<>::GetType(serializeData);

    using enum MsgType;
    switch (type)
    {
    case GameStarted:
    {
        const Message<GameStarted> message(serializeData);

        m_PlayerManager.CreateNewPlayer(message.PlayerX, sf::Color(250, 92, 12), Piece::X);
        m_PlayerManager.CreateNewPlayer(message.PlayerO, sf::Color(255, 194, 0), Piece::O);

        m_IsPlayerTurn = message.StartPlayer == ClientApp::GetInstance().GetCurrentPlayer()->GetName();

        m_GameStateUI->UpdateGameStateText("It's " + message.StartPlayer + " to start the game!");
        m_IsGameStarted = true;
        m_WaitingServerResponse = false;
        m_ScoreManager.InitPlayerScores(m_PlayerManager.GetAllPlayers());
        m_GameStateUI->InitPlayerScores(m_PlayerManager.GetAllPlayers());

        if (message.GameMode == FAST)
        {
            m_IsTimerOn = true;
            m_MaxPlayerTurnTime = GAMEMODE_FAST.PlayerMoveLimitTime;
            m_PlayerTurnTime = m_MaxPlayerTurnTime;
            
            m_GameStateUI->InitProgressBar(m_MaxPlayerTurnTime);
        }

        break;
    }
    case AcceptMakeMove:
    {
        const Message<AcceptMakeMove> message(serializeData);

        m_Board.InstanciateNewPlayerShape(message.Piece, message.Cell);
        SwitchPlayerTurn();

        m_WaitingServerResponse = false;

        break;
    }
    case DeclineMakeMove:
    {
        DebugLog("Declined move");
        m_WaitingServerResponse = false;
        break;
    }
    case GameOver:
    {
        const Message<GameOver> message(serializeData);

        m_WaitingServerResponse = false;

        if (message.IsDraw)
        {
            m_GameStateUI->UpdateGameStateText("Draw!");
        }
        else 
        {
            m_ScoreManager.AddScoreToPlayer(message.Piece);

            m_GameStateUI->UpdatePlayerScore(message.Piece, message.Winner, m_ScoreManager.GetPlayerScore(message.Piece));
            m_GameStateUI->UpdateGameStateText(message.Winner + " won!");
        }

        m_NeedToCleanBoard = true;

        break;
    }
    default:
       break;
    }
}


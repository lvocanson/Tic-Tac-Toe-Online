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
    Message<MsgType::OnEnterLobby> message;
    message.LobbyId = m_LobbyID;
    ClientConnectionHandler::GetInstance().SendDataToServer(message);

    m_ScoreManager.Init();
    m_GameStateUI->Init();
    m_GameStateUI->SetLobbyIDText(m_LobbyID);
    m_GameStateUI->SetUserName(ClientApp::GetInstance().GetCurrentPlayer()->GetName());

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
        Message<MsgType::LeaveLobby> message;
        message.LobbyId = m_LobbyID;
        message.PlayerName = ClientApp::GetInstance().GetCurrentPlayer()->GetName();
        ClientConnectionHandler::GetInstance().SendDataToServer(message);

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

    m_Window->ClearAllDrawables();
}

void GameState::OnReceiveData(const Json& serializeData)
{
    auto type = Message<>::GetType(serializeData);

    using enum MsgType;
    switch (type)
    {
    case GameStarted:
    {
        Message<GameStarted> message(serializeData);

        m_PlayerManager.CreateNewPlayer(message.PlayerX, sf::Color(250, 92, 12), Piece::X);
        m_PlayerManager.CreateNewPlayer(message.PlayerO, sf::Color(255, 194, 0), Piece::O);

        m_IsPlayerTurn = message.StartPlayer == ClientApp::GetInstance().GetCurrentPlayer()->GetName();

        m_GameStateUI->UpdateGameStateText("It's " + message.StartPlayer + " to start the game!");
        m_IsGameStarted = true;
        m_ScoreManager.InitPlayerScores(m_PlayerManager.GetAllPlayers());
        // m_GameStateUI->InitPlayerScores(m_PlayerManager.GetAllPlayers());

        break;
    }
    case AcceptMakeMove:
    {
        Message<AcceptMakeMove> message(serializeData);

        PlacePlayerPieceOnBoard(message.Cell, message.Piece);
        SwitchPlayerTurn();
        break;

    }
    case DeclineMakeMove:
    {
        DebugLog("Declined move");
        break;
    }
    case GameOver:
    {
        Message<GameOver> message(serializeData);

        //ClientApp::GetHistoryManager()->SaveGame(winner->GetData(), m_ScoreManager.GetCurrentGame());

        m_ScoreManager.CreateNewGameHistory();

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

        ClearBoard();

        break;

    }
    default:
       break;
    }
}


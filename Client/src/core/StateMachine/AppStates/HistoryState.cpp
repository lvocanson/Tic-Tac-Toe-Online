#include "HistoryState.h"
#include "src/core/ClientApp.h"
#include "src/core/Managers/GameHistoryManager.h"


HistoryState::HistoryState(StateMachine* stateMachine, Window* m_Window)
    : State(stateMachine)
    , m_Window(m_Window)
{
}

HistoryState::~HistoryState()
{
    NULLPTR(m_Window);
}

void HistoryState::OnEnter()
{
    m_NextMoveButton = new ButtonComponent(sf::Vector2f(750, 50), sf::Vector2f(50, 50), sf::Color::Green);
    m_NextMoveButton->SetButtonText(">", sf::Color::White, 30, TextAlignment::Center);
    m_NextMoveButton->SetOnClickCallback([this]()
    {
        PlacePiece();
    });

    m_PreviousMoveButton = new ButtonComponent(sf::Vector2f(450, 50), sf::Vector2f(50, 50), sf::Color::Green);
    m_PreviousMoveButton->SetButtonText("<", sf::Color::White, 30, TextAlignment::Center);
    m_PreviousMoveButton->SetOnClickCallback([this]()
    {
        RemovePiece();
    });

    m_BackToMenu = new ButtonComponent(sf::Vector2f(1000, 600), sf::Vector2f(200, 100), sf::Color::Blue);
    m_BackToMenu->SetButtonText("Back to Menu", sf::Color::White, 30, TextAlignment::Center);
    m_BackToMenu->SetOnClickCallback([this]()
    {
        m_StateMachine->SwitchState("MenuState");
    });

    m_NextGameButton = new ButtonComponent(sf::Vector2f(350, 50), sf::Vector2f(50, 50), sf::Color::Green);
    m_NextGameButton->SetButtonText(">", sf::Color::White, 30, TextAlignment::Center);
    m_NextGameButton->SetOnClickCallback([this]()
    {
        if (m_CurrentGameIndex + 1 < ClientApp::GetHistoryManager()->GetGameHistorySize())
        {
            m_CurrentGameIndex++;
            m_Board.SetEmpty();
            DisplayGame();
        }
        else
        {
            DebugLog("Invalid game index: " + std::to_string(m_CurrentGameIndex));
        }
    });

    m_PreviousGameButton = new ButtonComponent(sf::Vector2f(50, 50), sf::Vector2f(50, 50), sf::Color::Green);
    m_PreviousGameButton->SetButtonText("<", sf::Color::White, 30, TextAlignment::Center);
    m_PreviousGameButton->SetOnClickCallback([this]()
    {
        if (m_CurrentGameIndex - 1 > 0)
        {
            m_CurrentGameIndex--;
            m_Board.SetEmpty();
            DisplayGame();
        }
        else
        {
            DebugLog("Invalid game index: " + std::to_string(m_CurrentGameIndex));
        }
    });

    m_Board.Init(3, 3, m_Window);
    m_Board.DrawBoard();

    m_Window->RegisterDrawable(m_NextGameButton);
    m_Window->RegisterDrawable(m_PreviousGameButton);
    m_Window->RegisterDrawable(m_NextMoveButton);
    m_Window->RegisterDrawable(m_PreviousMoveButton);
    m_Window->RegisterDrawable(m_BackToMenu);
}

void HistoryState::OnUpdate(float dt)
{
    m_NextMoveButton->Update(dt);
    m_PreviousMoveButton->Update(dt);
    m_NextGameButton->Update(dt);
    m_PreviousGameButton->Update(dt);

    m_BackToMenu->Update(dt);
}

void HistoryState::OnExit()
{
    m_Board.SetEmpty();
    m_Board.ClearBoardShapes();
    m_Window->ClearAllDrawables();
}

void HistoryState::DisplayGame()
{
    if (m_CurrentGameIndex < ClientApp::GetHistoryManager()->GetGameHistorySize())
    {
        m_CurrentGame = ClientApp::GetHistoryManager()->GetGameData(m_CurrentGameIndex);
        m_CurrentMoveIndex = m_CurrentGame->GetMovesSize() - 1;

        for (const auto move : *m_CurrentGame->GetMoves())
        {
            m_Board.InstanciateNewPlayerShape(move->playerData.Id, move->playerData.ShapeType, move->BoardCell);
        }
    }
    else
    {
        DebugLog("Invalid game index: " + std::to_string(m_CurrentGameIndex));
    }
}

void HistoryState::PlacePiece()
{
    if (!m_CurrentGame) return;

    if (m_CurrentMoveIndex + 1 < m_CurrentGame->GetMovesSize())
    {
        m_CurrentMoveIndex++;
        const auto move = m_CurrentGame->GetMove(m_CurrentMoveIndex);
        m_Board.InstanciateNewPlayerShape(move->playerData.Id, move->playerData.ShapeType, move->BoardCell);
    }
    else
    {
        DebugLog("Invalid move index: " + std::to_string(m_CurrentMoveIndex));
    }
}

void HistoryState::RemovePiece()
{
    if (!m_CurrentGame) return;

    if (m_CurrentMoveIndex > 0)
    {
        m_CurrentMoveIndex--;
        m_Board.RemoveLastPlayerShape();
    }
    else
    {
        DebugLog("Invalid move index: " + std::to_string(m_CurrentMoveIndex));
    }
}

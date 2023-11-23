#include "HistoryState.h"
#include "src/core/ClientApp.h"
#include "src/core/Managers/GameHistoryManager.h"

const sf::Vector2f NEXT_BUTTON_OFFSET = sf::Vector2f(250, 0);

HistoryState::HistoryState(StateMachine* stateMachine, Window* m_Window)
    : State(stateMachine)
    , m_Window(m_Window)
{
    m_Games = std::vector<GameData*>();
}

HistoryState::~HistoryState()
{
    NULLPTR(m_Window);
}

void HistoryState::OnEnter()
{
    m_Games = ClientApp::GetHistoryManager()->GetAllGameData();

    m_CurrentGameIndex = 0;
    m_CurrentMoveIndex = 0;
    m_CurrentGame = nullptr;

    m_PreviousMoveButton = new ButtonComponent(sf::Vector2f(500, 50), sf::Vector2f(50, 50), sf::Color::Green);
    m_PreviousMoveButton->SetButtonText("<", sf::Color::White, 30, TextAlignment::Center);
    m_PreviousMoveButton->SetOnClickCallback([this]()
    {
        RemovePiece();
    });

    m_NextMoveButton = new ButtonComponent(m_PreviousMoveButton->GetPosition() + NEXT_BUTTON_OFFSET, sf::Vector2f(50, 50), sf::Color::Green);
    m_NextMoveButton->SetButtonText(">", sf::Color::White, 30, TextAlignment::Center);
    m_NextMoveButton->SetOnClickCallback([this]()
    {
        PlacePiece();
    });

    m_BackToMenu = new ButtonComponent(sf::Vector2f(1000, 600), sf::Vector2f(200, 100), sf::Color::Blue);
    m_BackToMenu->SetButtonText("Back to Menu", sf::Color::White, 30, TextAlignment::Center);
    m_BackToMenu->SetOnClickCallback([this]()
    {
        m_StateMachine->SwitchState("MenuState");
    });

    m_PreviousGameButton = new ButtonComponent(sf::Vector2f(50, 50), sf::Vector2f(50, 50), sf::Color::Green);
    m_PreviousGameButton->SetButtonText("<", sf::Color::White, 30, TextAlignment::Center);
    m_PreviousGameButton->SetOnClickCallback([this]()
    {
        PreviousGame();
    });

    m_NextGameButton = new ButtonComponent(m_PreviousGameButton->GetPosition() + NEXT_BUTTON_OFFSET, sf::Vector2f(50, 50), sf::Color::Green);
    m_NextGameButton->SetButtonText(">", sf::Color::White, 30, TextAlignment::Center);
    m_NextGameButton->SetOnClickCallback([this]()
    {
        NextGame();
    });

    m_MoveNumberText = new TextComponent;
    m_MoveNumberText->SetText("0 / 0");
    sf::Vector2f position = m_PreviousMoveButton->GetPosition() + sf::Vector2f((m_NextMoveButton->GetPosition().x - m_PreviousMoveButton->GetPosition().x) * 0.5f, 0);
    m_MoveNumberText->SetPosition(position);
    m_MoveNumberText->SetCharacterSize(46);
    m_MoveNumberText->SetColor(sf::Color::White);

    position = m_PreviousGameButton->GetPosition() + sf::Vector2f((m_NextGameButton->GetPosition().x - m_PreviousGameButton->GetPosition().x) * 0.5f, 0);
    m_GameNumberText = new TextComponent;
    m_GameNumberText->SetText("0 / " + std::to_string(m_Games.size()));
    m_GameNumberText->SetPosition(position);
    m_GameNumberText->SetCharacterSize(46);
    m_GameNumberText->SetColor(sf::Color::White);

    m_GameButtonLabel = new TextComponent;
    m_GameButtonLabel->SetText("GAME HISTORY");
    m_GameButtonLabel->SetPosition(m_GameNumberText->GetPosition() - sf::Vector2f(m_GameButtonLabel->GetSize().x * 0.5f + 10, 50));
    m_GameButtonLabel->SetCharacterSize(40);

    m_MoveButtonLabel = new TextComponent;
    m_MoveButtonLabel->SetText("PLAYER MOVE");
    m_MoveButtonLabel->SetPosition(m_MoveNumberText->GetPosition() - sf::Vector2f(m_MoveButtonLabel->GetSize().x * 0.5f + 10, 50));
    m_MoveButtonLabel->SetCharacterSize(40);

    m_GameWinnerText = new TextComponent;
    m_GameWinnerText->SetText("GAME WINNER");

    m_Board.Init(3, 3, m_Window);
    m_Board.DrawBoard();

    m_Window->RegisterDrawable(m_MoveButtonLabel);
    m_Window->RegisterDrawable(m_GameButtonLabel);
    m_Window->RegisterDrawable(m_NextGameButton);
    m_Window->RegisterDrawable(m_PreviousGameButton);
    m_Window->RegisterDrawable(m_NextMoveButton);
    m_Window->RegisterDrawable(m_PreviousMoveButton);
    m_Window->RegisterDrawable(m_BackToMenu);
    m_Window->RegisterDrawable(m_MoveNumberText);
    m_Window->RegisterDrawable(m_GameNumberText);
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

void HistoryState::DisplaySelectedGame()
{
    m_CurrentGame = ClientApp::GetHistoryManager()->GetGameData(m_CurrentGameIndex);
    m_CurrentMoveIndex = m_CurrentGame->GetMovesSize() - 1;

    for (const auto move : *m_CurrentGame->GetMoves())
    {
        m_Board.InstanciateNewPlayerShape(move->PlayerPiece, move->BoardCell);
    }

}

void HistoryState::NextGame()
{
    if (m_CurrentGameIndex + 1 <= m_Games.size())
    {
        m_CurrentGameIndex++;
        m_Board.SetEmpty();
        DisplaySelectedGame();
        m_GameNumberText->SetText(std::to_string(m_CurrentGameIndex) + " / " + std::to_string(m_Games.size()));
    }
    else
    {
        DebugLog("Invalid game index: " + std::to_string(m_CurrentGameIndex));
    }
}

void HistoryState::PreviousGame()
{
    if (m_CurrentGameIndex > 0)
    {
        m_CurrentGameIndex--;
        m_Board.SetEmpty();
        DisplaySelectedGame();
        m_GameNumberText->SetText(std::to_string(m_CurrentGameIndex) + " / " + std::to_string(m_Games.size()));
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
        m_Board.InstanciateNewPlayerShape(move->PlayerPiece, move->BoardCell);
        m_MoveNumberText->SetText(std::to_string(m_CurrentMoveIndex) + " / " + std::to_string(m_CurrentGame->GetMovesSize()));
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
        m_MoveNumberText->SetText(std::to_string(m_CurrentMoveIndex) + " / " + std::to_string(m_CurrentGame->GetMovesSize()));
    }
    else
    {
        DebugLog("Invalid move index: " + std::to_string(m_CurrentMoveIndex));
    }
}

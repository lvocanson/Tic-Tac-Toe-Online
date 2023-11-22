#include "HistoryState.h"
#include "src/core/ClientApp.h"
#include "TicTacToe.h"
#include "src/core/Managers/GameHistoryManager.h"

HistoryState::HistoryState(StateMachine* stateMachine, Window* m_Window)
	: State(stateMachine), m_Window(m_Window), m_RArrowButton(), m_LArrowButton(), m_BackToMenu()
{
	m_CurrentGameIndex = 0;
	m_CurrentMoveIndex = 0;
}

HistoryState::~HistoryState()
{
	NULLPTR(m_Window);
}

void HistoryState::OnEnter()
{
	m_RArrowButton = new ButtonComponent(sf::Vector2f(750, 50), sf::Vector2f(50, 50), sf::Color::Green, sf::Color::Red);
	m_RArrowButton->SetButtonText(">", sf::Color::White, 30, TextAlignment::Center);
	m_RArrowButton->SetOnClickCallback([this]()
		{ PlacePiece(); });

	m_LArrowButton = new ButtonComponent(sf::Vector2f(450, 50), sf::Vector2f(50, 50), sf::Color::Green, sf::Color::Red);
	m_LArrowButton->SetButtonText("<", sf::Color::White, 30, TextAlignment::Center);
	m_LArrowButton->SetOnClickCallback([this]()
		{ RemovePiece(); });

	m_BackToMenu = new ButtonComponent(sf::Vector2f(1000, 600), sf::Vector2f(200, 100), sf::Color::Blue, sf::Color::Red);
	m_BackToMenu->SetButtonText("Back to Menu", sf::Color::White, 30, TextAlignment::Center);
	m_BackToMenu->SetOnClickCallback([this]()
		{ m_StateMachine->SwitchState("MenuState"); });

	m_Board.Init(3, 3, m_Window);
	m_Board.DrawBoard();

	RenderHistory();

	m_Window->RegisterDrawable(m_RArrowButton);
	m_Window->RegisterDrawable(m_LArrowButton);
	m_Window->RegisterDrawable(m_BackToMenu);
}

void HistoryState::OnUpdate(float dt)
{
	m_RArrowButton->Update();
	m_LArrowButton->Update();
	m_BackToMenu->Update();

	for (const auto historyButton : m_HistoryButtons)
	{
		historyButton->Update();
	}
}

void HistoryState::OnExit()
{
	m_HistoryButtons.clear();
	m_Board.SetEmpty();
	m_Board.ClearBoardShapes();
	m_Window->ClearAllDrawables();
}

void HistoryState::RenderHistory()
{
	sf::Vector2f displayPosition(150.0f, 50.0f);
	const float verticalSpacing = 75.0f;

	for (size_t i = 0; i < ClientApp::GetHistoryManager()->GetGameHistorySize(); i++)
	{
		auto historyButton = new ButtonComponent(
			sf::Vector2f(50, 50 + verticalSpacing * i),
			displayPosition,
			sf::Color::White,
			sf::Color::Green);

		historyButton->SetButtonText("Game " + std::to_string(i + 1), sf::Color::Black, 20, TextAlignment::Center);

		historyButton->SetOnClickCallback([this, i]()
			{
				m_CurrentGameIndex = i;
				m_Board.SetEmpty();
				DisplayGame();
			});

		m_Window->RegisterDrawable(historyButton);
		m_HistoryButtons.push_back(historyButton);
	}
}

void HistoryState::DisplayGame()
{
	if (m_CurrentGameIndex < ClientApp::GetHistoryManager()->GetGameHistorySize())
	{
		m_CurrentGame = ClientApp::GetHistoryManager()->GetGameData(m_CurrentGameIndex);
		m_CurrentMoveIndex = m_CurrentGame->GetMovesSize() - 1;

		for (const auto move : *m_CurrentGame->GetMoves())
		{
			m_Board.InstanciateNewPlayerShape(move->playerData.Id, move->BoardCell);
		}
	}
	else
	{
		std::cout << "Invalid game index: " << m_CurrentGameIndex << std::endl;
	}
}

void HistoryState::PlacePiece()
{
	if (m_CurrentMoveIndex + 1 < m_CurrentGame->GetMovesSize())
	{
		m_CurrentMoveIndex++;
		const auto move = m_CurrentGame->GetMove(m_CurrentMoveIndex);
		m_Board.InstanciateNewPlayerShape(move->playerData.Id, move->BoardCell);
	}
	else
	{
		std::cout << "Invalid move index: " << m_CurrentMoveIndex << std::endl;
	}
}

void HistoryState::RemovePiece()
{
	if (m_CurrentMoveIndex > 0)
	{
		m_CurrentMoveIndex--;
		m_Board.RemoveLastPlayerShape();
	}
	else
	{
		std::cout << "Invalid move index: " << m_CurrentMoveIndex << std::endl;
	}
}

#pragma once
#include "src/core/StateMachine/StateMachine.h"
#include "src/core/Window.h"
#include "src/core/GraphicBoard.h"
#include "src/core/Components/ButtonComponent.h"

#include "src/core/Managers/GameHistoryManager.h"
#include "src/core/Managers/PlayerManager.h"
#include "src/core/PlayerPieceShape.h"


class HistoryState : public State
{
public:
	void OnEnter() override;
	void OnUpdate(float dt) override;
	void OnExit() override;

	HistoryState(StateMachine* stateMachine, Window* m_Window);
	HistoryState(const HistoryState& other) = delete;
	HistoryState& operator=(const HistoryState& other) = delete;
	~HistoryState();

	void RenderHistory();

	void DisplayGame();

	void PlacePiece();
	void RemovePiece();

	void ClearBoard();

private:
	Window* m_Window;

	ButtonComponent* m_RArrowButton;
	ButtonComponent* m_LArrowButton;
	ButtonComponent* m_BackToMenu;

	GraphicBoard m_Board;

	PlayerPieceShape m_PlayerPieceShape;
	GameHistoryManager m_GameHistoryManager;
	PlayerManager m_PlayerManager;

	std::vector<ButtonComponent*> m_HistoryButtons;


	size_t m_CurrentGameIndex;
};
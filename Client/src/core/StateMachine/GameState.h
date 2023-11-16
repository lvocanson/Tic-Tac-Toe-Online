#pragma once
#include "StateMachine.h"
#include "State.h"
#include "src/core/Components/ButtonComponent.h"
#include "src/core/GraphicBoard.h"
#include "src/core/Player.h"

#include "src/core/Managers/PlayerManager.h"
#include "src/core/Managers/GameStateUI.h"
#include "src/core/Managers/ScoreManager.h"
#include "src/core/Managers/UIStateManager.h"

class sf::Shape;

class GameState : public State
{
public:
	void OnEnter() override;
	void OnUpdate(float dt) override;
	void OnExit() override;

	GameState(StateMachine* stateMachine, Window* m_Window);
	GameState(const GameState& other) = delete;
	GameState& operator=(const GameState& other) = delete;
	~GameState();

	void DrawBoard();

	void CheckIfMouseHoverBoard();
	bool IsMouseHoverPiece(unsigned int i);
	void PlacePlayerPieceOnBoard(unsigned int cell);
	void SetGraphicalPiece(unsigned cell, const Player* currentPlayer);
	void SwitchPlayerTurn();

	void ClearBoard();

private:
	Window* m_Window;
	ButtonComponent* m_ReturnButton;

	GraphicBoard m_Board;

	PlayerManager m_PlayerManager;
	ScoreManager m_ScoreManager;

	sf::Time m_PlayerTurnTimer = sf::seconds(0);

	std::vector<sf::Drawable*> m_GamePieces;

	GameStateUI* m_GameStateUI;
};
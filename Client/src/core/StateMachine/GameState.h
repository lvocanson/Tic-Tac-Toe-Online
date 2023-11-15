#pragma once
#include "StateMachine.h"
#include "State.h"
#include "src/core/Components/ButtonComponent.h"
#include "src/core/GraphicBoard.h"
#include "src/core/Player.h"
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

	void DrawBoard();

	void SwitchPlayerTurn();
	void PlacePlayerPieceOnBoard(unsigned int i);

	void ClearBoard();
	void CheckIfMouseHoverBoard();
	bool IsMouseHoverPiece(unsigned int i);

private:
	Window* m_Window;
	ButtonComponent* m_ButtonComponent;

	GraphicBoard m_Board;
	Player m_PlayerOne;
	Player m_PlayerTwo;

	sf::Time m_PlayerTurnTimer = sf::seconds(0);

	std::vector<sf::Drawable*> m_GamePieces;

	bool m_IsPlayerOneTurn = true;
};
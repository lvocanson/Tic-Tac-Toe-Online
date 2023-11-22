#pragma once
#include "src/core/StateMachine/StateMachine.h"
#include "src/core/Components/ButtonComponent.h"
#include "src/core/GraphicBoard.h"
#include "src/core/Player.h"
#include "src/core/UIState/GameStateUI.h"

#include "src/core/Managers/PlayerManager.h"
#include "src/core/Managers/ScoreManager.h"
#include "src/core/Managers/UIStateManager.h"

#include "game/Lobby.h"

class sf::Shape;

class GameState : public State
{
public:
	void OnEnter() override;
    void OnUpdate(float dt) override;
	void OnExit() override;
	void OnReceiveData(const Json& serializeData) override;

	GameState(StateMachine* stateMachine, Window* m_Window);
	GameState(const GameState& other) = delete;
	GameState& operator=(const GameState& other) = delete;
	~GameState() override;

	void SetLobbyID(unsigned int id) { m_LobbyID = id; }

private:


	void DrawBoard();
	void UpdatePlayerTimer(float dt);
	void CheckIfTimerIsUp();

    void CheckIfMouseHoverBoard();
	bool IsMouseHoverPiece(unsigned int i);
	void PlacePlayerPieceOnBoard(unsigned int cell);
	void InstanciateNewPlayerShape(const Player* currentPlayer, const unsigned int cell);
	void WinCheck();
	void SwitchPlayerTurn();

	void SendPlacedPieceToServer(unsigned int cell);

	void ClearBoard();

private:

	unsigned int m_LobbyID;

	Window* m_Window;
	ButtonComponent* m_ReturnButton;
	bool m_IsPlayersConnected = false;
	bool m_IsGameInit = false;
	GraphicBoard m_Board;

	PlayerManager m_PlayerManager;
	ScoreManager m_ScoreManager;

	float m_PlayerTurnTime;
	float m_MaxPlayerTurnTime;
	bool m_IsTimerOn;

	std::vector<sf::Drawable*> m_GamePieces;

	GameStateUI* m_GameStateUI;
};
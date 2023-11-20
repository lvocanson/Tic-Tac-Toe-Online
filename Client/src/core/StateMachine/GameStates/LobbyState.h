#pragma once
#include "src/core/StateMachine/StateMachine.h"
#include "src/core/Window.h"
#include "src/core/Components/ButtonComponent.h"
#include "src/core/Managers/PlayerManager.h"
#include "src/core/UIState/GameStateUI.h"

class LobbyState : public State
{
public:
	void OnEnter() override;
	void OnUpdate(float dt) override;
	void OnExit() override;

	LobbyState(StateMachine* stateMachine, Window* window);
	LobbyState(const LobbyState& other) = delete;
	LobbyState& operator=(const LobbyState& other) = delete;
	~LobbyState();

	void JoinLobby();
	void LeaveLobby();

private:
	

private:
	Window* m_Window;

	ButtonComponent* m_ReturnButton;
	ButtonComponent* m_LobbyButton;
	GameStateUI* m_GameStateUI;
};
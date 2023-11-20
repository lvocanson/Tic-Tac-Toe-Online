#pragma once
#include "src/core/StateMachine/StateMachine.h"
#include "src/core/Window.h"
#include "src/core/Components/ButtonComponent.h"
#include "src/core/Managers/PlayerManager.h"

struct Lobby
{
	int ID;
	int PlayerCount;
	const int MaxPlayerCount = 2;
};

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

private:
	void CreateLobbies();
	bool IsLobbyFull(Lobby& lb);

private:
	int m_MaxLobbyNumber;
	bool m_IsLobbyCreated;

	Lobby* m_CurrentLobby;

	Window* m_Window;

	ButtonComponent* m_ReturnButton;

	std::vector<Lobby*> m_Lobbies;
	std::vector<ButtonComponent*> m_LobbyButtons;
};
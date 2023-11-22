#pragma once
#include "src/core/StateMachine/StateMachine.h"
#include "src/core/Window.h"
#include "src/core/Components/ButtonComponent.h"
#include "src/core/Managers/PlayerManager.h"
#include "src/core/UIState/GameStateUI.h"
#include "game/Lobby.h"

class LobbyState : public State
{
public:
    void OnEnter() override;
    void OnUpdate(float dt) override;
    void OnExit() override;
    void OnReceiveData(const Json& serializeData) override;

    LobbyState(StateMachine* stateMachine, Window* window);
    LobbyState(const LobbyState& other) = delete;
    LobbyState& operator=(const LobbyState& other) = delete;
    ~LobbyState();

    void TryToJoinLobby(int lobbyID);
    void LeaveLobby();

private:

    int m_CurrentLobbyID;
    bool m_IsLobbyInit = false;
    bool m_IsInLobby = false;


    Window* m_Window;
    GameStateUI* m_GameStateUI;
    ButtonComponent* m_ReturnButton;
    ButtonComponent* m_LeaveButtons = nullptr;

    std::vector<ButtonComponent*> m_LobbyButtons;
    std::vector<Lobby> m_Lobbies;
    std::string m_PlayerName;
};

#pragma once
#include "src/core/StateMachine/StateMachine.h"
#include "src/core/Window.h"
#include "src/core/Components/ButtonComponent.h"
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
    ~LobbyState() override;

    void JoinLobbyRequest(int lobbyID);

private:

    int m_CurrentLobbyID;
    bool m_IsLobbyInit = false;
    bool m_IsInLobby = false;
    bool m_IsTryingToJoinLobby = false;


    Window* m_Window = nullptr;
    GameStateUI* m_GameStateUI = nullptr;
    ButtonComponent* m_ReturnButton = nullptr;
    ButtonComponent* m_LeaveButtons = nullptr;
    ButtonComponent* m_HistoryButton = nullptr;

    std::string m_LobbyGameMode;
    std::vector<ButtonComponent*> m_LobbyButtons;
    std::vector<LobbyData> m_Lobbies;
};

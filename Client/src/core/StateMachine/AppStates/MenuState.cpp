#include "MenuState.h"
#include "src/core/ClientApp.h"

MenuState::MenuState(StateMachine* stateMachine, Window* window)
    : State(stateMachine)
    , m_Window(window)
{
}

MenuState::~MenuState()
{
    NULLPTR(m_Window);
}

void MenuState::OnEnter()
{
    if (ClientConnectionHandler::GetInstance().IsConnected())
    {
        m_PlayButton = new ButtonComponent(sf::Vector2f(100, 100), sf::Vector2f(200, 100), sf::Color::Blue);
        m_PlayButton->SetButtonText("Play", sf::Color::White, 30, TextAlignment::Center);
        m_PlayButton->SetOnClickCallback([this]()
            {
                m_StateMachine->SwitchState("LobbyState");
            });
        m_Window->RegisterDrawable(m_PlayButton);

        m_DisconnectButton = new ButtonComponent(sf::Vector2f(300, 500), sf::Vector2f(200, 100), sf::Color::Red);
        m_DisconnectButton->SetButtonText("Disconnect", sf::Color::White, 30, TextAlignment::Center);
        m_DisconnectButton->SetOnClickCallback([this]()
            {
                ClientConnectionHandler::GetInstance().Disconnect();
                m_Window->UnregisterDrawable(m_PlayButton);
                RELEASE(m_PlayButton);
                ShowConnectionButton();
            });

        m_Window->RegisterDrawable(m_DisconnectButton);
    }
    else
    {
        ShowConnectionButton();
    }

    m_HistoryButton = new ButtonComponent(sf::Vector2f(100, 300), sf::Vector2f(200, 100), sf::Color::Green);
    m_HistoryButton->SetButtonText("History", sf::Color::White, 30, TextAlignment::Center);
    m_HistoryButton->SetOnClickCallback([this]()
        {
            m_StateMachine->SwitchState("HistoryState");
        });

    m_QuitButton = new ButtonComponent(sf::Vector2f(100, 500), sf::Vector2f(200, 100), sf::Color::Red);
    m_QuitButton->SetButtonText("Quit", sf::Color::White, 30, TextAlignment::Center);
    m_QuitButton->SetOnClickCallback([this]()
        {
            ClientApp::GetInstance().Shutdown();
        });

    m_Window->RegisterDrawable(m_HistoryButton);
    m_Window->RegisterDrawable(m_QuitButton);
}

void MenuState::ShowConnectionButton()
{
    m_PlayButton = new ButtonComponent(sf::Vector2f(100, 100), sf::Vector2f(200, 100), sf::Color::Blue);
    m_PlayButton->SetButtonText("Connection", sf::Color::White, 30, TextAlignment::Center);
    m_PlayButton->SetOnClickCallback([this]()
        {
            m_StateMachine->SwitchState("ConnectionState");
        });
    m_Window->RegisterDrawable(m_PlayButton);
}


void MenuState::OnUpdate(float dt)
{
    m_PlayButton->Update(dt);
    m_HistoryButton->Update(dt);
    m_QuitButton->Update(dt);

    if (m_DisconnectButton)
        m_DisconnectButton->Update(dt);
}

void MenuState::OnExit()
{
    m_Window->UnregisterDrawable(m_PlayButton);
    m_Window->UnregisterDrawable(m_HistoryButton);
    m_Window->UnregisterDrawable(m_QuitButton);

    if (m_DisconnectButton)
        m_Window->UnregisterDrawable(m_DisconnectButton);

    RELEASE(m_PlayButton);
    RELEASE(m_HistoryButton);
    RELEASE(m_QuitButton);
    RELEASE(m_DisconnectButton);
}

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
        sf::Color Emerald(1, 215, 88);
        m_ConnectButton = new ButtonComponent(sf::Vector2f(m_Window->GetWidth() * 0.5f - 150, 100), sf::Vector2f(200, 100), Emerald);
        m_ConnectButton->SetButtonText("Play", sf::Color::White, 50, TextAlignment::Center);
        m_ConnectButton->SetOnClickCallback([this]()
            {
                m_StateMachine->SwitchState("LobbyState");
            });
        m_Window->RegisterDrawable(m_ConnectButton);

        m_DisconnectButton = new ButtonComponent(sf::Vector2f(m_Window->GetWidth() * 0.5f - 150, 500), sf::Vector2f(200, 100), sf::Color::Red);
        m_DisconnectButton->SetButtonText("Disconnect", sf::Color::White, 50, TextAlignment::Center);
        m_DisconnectButton->SetOnClickCallback([this]()
            {
                ClientConnectionHandler::GetInstance().Disconnect();
                m_Window->UnregisterDrawable(m_ConnectButton);
                RELEASE(m_ConnectButton);
                ShowConnectionButton();
                m_Window->UnregisterDrawable(m_DisconnectButton);
            });

        m_Window->RegisterDrawable(m_DisconnectButton);
    }
    else
    {
        ShowConnectionButton();
    }
    
    sf::Color GreenDuck(4, 139, 154);
    m_HistoryButton = new ButtonComponent(sf::Vector2f(m_Window->GetWidth() * 0.5f - 150, 300), sf::Vector2f(200, 100), GreenDuck);
    m_HistoryButton->SetButtonText("History", sf::Color::White, 50, TextAlignment::Center);
    m_HistoryButton->SetOnClickCallback([this]()
        {
            m_StateMachine->SwitchState("HistoryState");
        });

    sf::Color OrangeRed(231, 62, 1);
    m_QuitButton = new ButtonComponent(sf::Vector2f(m_Window->GetWidth() * 0.5f - 150, 500), sf::Vector2f(200, 100), OrangeRed);
    m_QuitButton->SetButtonText("Quit", sf::Color::White, 50, TextAlignment::Center);
    m_QuitButton->SetOnClickCallback([this]()
        {
            ClientApp::GetInstance().Shutdown();
        });

    m_Window->RegisterDrawable(m_HistoryButton);
    m_Window->RegisterDrawable(m_QuitButton);
}

void MenuState::ShowConnectionButton()
{
    sf::Color Carrot(244, 102, 27);
    m_ConnectButton = new ButtonComponent(sf::Vector2f(m_Window->GetWidth() * 0.5f - 150, 100), sf::Vector2f(200, 100), Carrot);
    m_ConnectButton->SetButtonText("Connection", sf::Color::White, 50, TextAlignment::Center);
    m_ConnectButton->SetOnClickCallback([this]()
        {
            m_StateMachine->SwitchState("ConnectionState");
        });
    m_Window->RegisterDrawable(m_ConnectButton);
}


void MenuState::OnUpdate(float dt)
{
    m_ConnectButton->Update(dt);
    m_HistoryButton->Update(dt);
    m_QuitButton->Update(dt);

    if (m_DisconnectButton)
        m_DisconnectButton->Update(dt);
}

void MenuState::OnExit()
{
    if (m_DisconnectButton)
    {
        m_Window->UnregisterDrawable(m_DisconnectButton);
        RELEASE(m_DisconnectButton);
    }

    m_Window->ClearAllDrawables();

    NULLPTR(m_ConnectButton);
    NULLPTR(m_HistoryButton);
    NULLPTR(m_QuitButton);
}

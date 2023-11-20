#include "ConnectionState.h"
#include "src/core/Managers/Resources/FontRegistry.h"
#include <regex>
#include "src/core/ClientApp.h"

ConnectionState::ConnectionState(StateMachine* stateMachine, Window* window)
    : State(stateMachine)
    , m_Window(window)
{
    m_StateMachine = stateMachine;
}

ConnectionState::~ConnectionState()
{
    NULLPTR(m_Window);
}

void ConnectionState::OnEnter()
{


    sf::Font font = *FontRegistry::GetFont("bold-font");

    sf::Color color(171, 171, 171);
    m_IpField = new InsertFieldComponent(sf::Vector2f(100, 300), sf::Vector2f(300, 50), color, sf::Color::White, 1.0f);

    m_BackButton = new ButtonComponent(sf::Vector2f(100, 400), sf::Vector2f(150, 50), sf::Color::Green, sf::Color::Red);
    m_BackButton->SetButtonText("Quit", sf::Color::White, 30, TextAlignment::Center);
    m_BackButton->SetOnClickCallback([this]() {
        m_StateMachine->SwitchState("MenuState");
        });

    m_Window->RegisterDrawable(m_IpField);
    m_Window->RegisterDrawable(m_BackButton);
}

void ConnectionState::OnUpdate(float dt)
{
    m_IpField->Update();
    m_BackButton->Update();

    if (InputHandler::IsKeyPressed(sf::Keyboard::Enter) && m_IpField != nullptr)
    {
        std::string ip = m_IpField->GetText();

        if (IsValidIpAddress(ip.c_str()))
        {
            ClientApp::GetInstance().Connection(ip.c_str());
			m_StateMachine->SwitchState("GameState");
        }
        else
        {
            DebugLog("Invalid IP address format!\n");
        }
    }
}


void ConnectionState::OnExit()
{
    m_Window->UnregisterDrawable(m_IpField);
    m_Window->UnregisterDrawable(m_BackButton);

    RELEASE(m_IpField);
    RELEASE(m_BackButton);
}

bool ConnectionState::IsValidIpAddress(const char* ip)
{
    const std::regex ipRegex("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$");
    return std::regex_match(ip, ipRegex);
}
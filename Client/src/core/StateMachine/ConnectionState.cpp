#include "ConnectionState.h"
#include "src/core/Managers/FontRegistry.h"

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

    m_Client = new TcpIpClient();

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

    if (InputHandler::IsKeyPressed(sf::Keyboard::Enter) && m_IpField != NULL)
    {
		//std::string ip = m_IpField->GetText();
        try
        {
            m_Client->Connect("localhost", DEFAULT_PORT);
			DebugLog("Connected to server!\n");
			m_Client->Send("Hello from client!");
			m_StateMachine->SwitchState("GameState");
		}
        catch (const TcpIp::TcpIpException& e)
        {
			DebugLog("Failed to connect to server: " + std::string(e.what()) + "\n");
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

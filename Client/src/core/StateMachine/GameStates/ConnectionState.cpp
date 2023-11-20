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
	m_IpField = new InsertFieldComponent();
	m_IpField->SetPosition(sf::Vector2f(100, 100));
	m_IpField->SetLabel("IP Address");

	m_NameField = new InsertFieldComponent();
	m_NameField->SetPosition(sf::Vector2f(100, 200));
	m_NameField->SetLabel("Username");

	m_BackButton = new ButtonComponent(sf::Vector2f(100, 400), sf::Vector2f(150, 50), sf::Color::Green, sf::Color::Red);
	m_BackButton->SetButtonText("Quit", sf::Color::White, 30, TextAlignment::Center);
	m_BackButton->SetOnClickCallback([this]() {
		m_StateMachine->SwitchState("MenuState");
	});

	m_ConnectButton = new ButtonComponent(sf::Vector2f(100, 500), sf::Vector2f(150, 50), sf::Color::Green, sf::Color::Red);
	m_ConnectButton->SetButtonText("Connect", sf::Color::White, 30, TextAlignment::Center);
	m_ConnectButton->SetOnClickCallback([this]()
	{
		if (m_NameField->GetText().empty())
		{
			DebugLog("Username should not be empty!\n");
			return;
		}
		else if (m_NameField->GetText().size() < 3)
		{
		    DebugLog("Username should be more than 2 characters!\n");
            return;
		}

        const std::string ip = m_IpField->GetText();

        if (IsValidIpAddress(ip.c_str()))
        {
			ClientApp::GetInstance().SetPlayerName(m_NameField->GetText());
            ClientApp::GetInstance().Connection(m_IpField->GetText());

			Json j;
			j["Type"] = "Login";
			j["UserName"] = m_NameField->GetText();
			ClientApp::GetInstance().Send(j.dump());
			//Switch state to lobby state later
            m_StateMachine->SwitchState("GameState");
        }
        else
        {
            DebugLog("Invalid IP address format!\n");
        }
    });	

	m_Window->RegisterDrawable(m_IpField);
	m_Window->RegisterDrawable(m_NameField);
	m_Window->RegisterDrawable(m_BackButton);
	m_Window->RegisterDrawable(m_ConnectButton);
}

void ConnectionState::OnUpdate(float dt)
{
	m_IpField->Update(dt);
	m_NameField->Update(dt);
	m_BackButton->Update(dt);
	m_ConnectButton->Update(dt);
}

void ConnectionState::OnExit()
{
	m_Window->UnregisterDrawable(m_IpField);
	m_Window->UnregisterDrawable(m_BackButton);
	m_Window->UnregisterDrawable(m_ConnectButton);
	m_Window->UnregisterDrawable(m_NameField);

	RELEASE(m_IpField);
	RELEASE(m_NameField);
	RELEASE(m_ConnectButton);
	RELEASE(m_BackButton);
}

bool ConnectionState::IsValidIpAddress(const char* ip)
{
	const std::regex ipRegex("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$");
	return std::regex_match(ip, ipRegex);
}
#include "InsertFieldComponent.h"
#include "src/core/Managers/FontRegistry.h"
#include "src/core/Managers/InputHandler.h"

InsertFieldComponent::InsertFieldComponent(const sf::Vector2f pos, const sf::Vector2f size, const sf::Color& idleColor, const sf::Color& hoverColor, float outlineThickness)
	: m_Focus(false)
	, m_HasLimit(false)
	, limit(0)
{
	m_Text.setFont(*FontRegistry::GetFont("bold-font"));

	m_Rectangle.setPosition(pos);
	m_Rectangle.setSize(size);
	m_Rectangle.setFillColor(idleColor);
	m_Rectangle.setOutlineColor(hoverColor);
	m_Rectangle.setOutlineThickness(outlineThickness);
}

InsertFieldComponent::~InsertFieldComponent()
{
}

void InsertFieldComponent::Update()
{
	if (IsMouseOver())
	{
		m_Rectangle.setOutlineThickness(2.0f);

		if (InputHandler::IsMouseButtonPressed(sf::Mouse::Left))
		{
			m_Focus = true;
		}
	}
	else
	{
		m_Rectangle.setOutlineThickness(0.0f);
	}

	if (m_Focus) 
	{
		if (InputHandler::IsKeyPressed(sf::Keyboard::BackSpace) && m_TextStream.str().size() > 0)
		{
			auto s = m_TextStream.str();
			s.pop_back();
			m_TextStream.str(s);
			m_Text.setString(m_TextStream.str());
		}

		if (InputHandler::IsKeyPressed(sf::Keyboard::Enter))
		{
			m_Focus = false;
		}

		if (InputHandler::IsKeyPressed(sf::Keyboard::A))
		{
			m_TextStream << 'a';
			m_Text.setString(m_TextStream.str());
		}
	}
}

void InsertFieldComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Rectangle, states);

	if (m_Text.getString() != "")
	{
		target.draw(m_Text, states);
	}
}

bool InsertFieldComponent::IsMouseOver()
{
	sf::Vector2f mousePos = (sf::Vector2f)InputHandler::GetMousePosition();
	sf::Vector2f buttonPos = m_Rectangle.getPosition();
	sf::Vector2f buttonSize = m_Rectangle.getSize();

	return mousePos.x >= buttonPos.x && mousePos.x <= buttonPos.x + buttonSize.x &&
		mousePos.y >= buttonPos.y && mousePos.y <= buttonPos.y + buttonSize.y;
}

void InsertFieldComponent::SetPosition(const sf::Vector2f& position)
{
	m_Rectangle.setPosition(position);

	if (m_Text.getString() != "")
	{
		m_Text.setPosition(position);
	}
}

sf::Vector2f InsertFieldComponent::GetPosition() const
{
	return m_Rectangle.getPosition();
}

sf::Vector2f InsertFieldComponent::GetSize() const
{
	return m_Rectangle.getSize();
}


void InsertFieldComponent::SetFocus(bool focus)
{
	m_Focus = focus;
}

bool InsertFieldComponent::GetFocus() const
{
	return m_Focus;
}

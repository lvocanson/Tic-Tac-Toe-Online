#pragma once
#include "BaseComponent.h"
#include "src/core/Managers/InputHandler.h"
#include <iostream>
#include <sstream>

class InsertFieldComponent : public BaseComponent
{
public:

	InsertFieldComponent(const sf::Vector2f pos, const sf::Vector2f size, const sf::Color& idleColor, const sf::Color& hoverColor, float outlineThickness);
	~InsertFieldComponent() override;

	void Update() override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void SetPosition(const sf::Vector2f& position) override;
	sf::Vector2f GetPosition() const override;
	sf::Vector2f GetSize() const override;

	void SetFocus(bool focus);
	bool GetFocus() const;

private:
	sf::RectangleShape m_Rectangle;
	sf::Text m_Text;
	std::ostringstream m_TextStream;
	bool m_Focus;
	bool m_HasLimit;
	int limit;

	bool IsMouseOver();
};

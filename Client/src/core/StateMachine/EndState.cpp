#include "EndState.h"

EndState::EndState(StateMachine* stateMachine, Window* m_Window) 
    : State(stateMachine)
    , m_Window(m_Window)
{
}

void EndState::OnEnter()
{
    //sf::Text* scoreText = new sf::Text("Score : " + std::to_string(m_Score), font, 20);
    //scoreText->setPosition(200, 200);

    //sf::RectangleShape* replayButton = new sf::RectangleShape(sf::Vector2f(150, 50));
    //replayButton->setPosition(100, 400);
    //replayButton->setFillColor(sf::Color::Green);
    //sf::Text* replayButtonText = new sf::Text("Rejouer", font, 20);
    //replayButtonText->setPosition(replayButton->getPosition().x + 20, replayButton->getPosition().y + 10);

    //sf::RectangleShape* menuButton = new sf::RectangleShape(sf::Vector2f(150, 50));
    //menuButton->setPosition(300, 400);
    //menuButton->setFillColor(sf::Color::Blue);
    //sf::Text* menuButtonText = new sf::Text("Menu principal", font, 20);
    //menuButtonText->setPosition(menuButton->getPosition().x + 10, menuButton->getPosition().y + 10);

    //m_Window->RegisterDrawable(scoreText);
    //m_Window->RegisterDrawable(replayButton);
    //m_Window->RegisterDrawable(replayButtonText);
    //m_Window->RegisterDrawable(menuButton);
    //m_Window->RegisterDrawable(menuButtonText);
}

void EndState::OnUpdate(float dt)
{

}

void EndState::OnExit()
{

}

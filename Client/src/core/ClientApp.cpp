#include "ClientApp.h"
#include "Player.h"
#include "TicTacToe.h"
#include "Window.h"

using namespace TicTacToe;

void ClientApp::Init()
{
    m_IsRunning = true;
    m_Window = new Window();
    m_Window->Create("Tic Tac Toe Online!", 800, 600);

    std::cout << "Hello World! I'm a client!\n";

    m_Board = new Board();

    //temp
    playerOneShape = new sf::CircleShape(30);
    playerOneShape->setFillColor(sf::Color::Transparent);
    playerOneShape->setOutlineThickness(10);
    playerOneShape->setOutlineColor(sf::Color::Red);

    //temp
    playerTwoShape = new sf::CircleShape(30);
    playerTwoShape->setFillColor(sf::Color::Transparent);
    playerTwoShape->setOutlineThickness(10);
    playerTwoShape->setOutlineColor(sf::Color::Yellow);

    m_Window->RegisterDrawable(playerOneShape);
    //m_Window->RegisterDrawable(playerTwoShape);

    m_PlayerOne = new Player("Player One", playerOneShape);
    m_PlayerTwo = new Player("Player Two", playerTwoShape);


    int pieceSize = m_Board->GetPieceSize();
    int width = m_Board->GetWidth();
    int height = m_Board->GetHeight();

    // Draw the board - temp
    for (size_t i = 0; i < m_Board->GetTotalSize(); ++i)
    {
	    auto* square = new sf::RectangleShape(sf::Vector2f(pieceSize, pieceSize));
		square->setFillColor(sf::Color::Transparent);
		square->setOutlineColor(sf::Color::Color(0, 189, 156));
		square->setOutlineThickness(5);
        square->setPosition(i % width * pieceSize, (i) / height * pieceSize);
		m_Window->RegisterDrawable(square);

        auto piece = m_Board->GetPieceAt(i);
        piece.SetPosition(square->getPosition());
        m_Window->RegisterDrawable(piece.GetShape());
	}

}

void ClientApp::Run()
{
    if (!m_IsRunning)
        throw std::runtime_error("ClientApp is not initialized!");

    sf::Clock clock;

    while (m_IsRunning && !m_IsGameFinished)
    {
        const sf::Time elapsed = clock.restart();

        m_Window->PollEvents();
        Update(elapsed);
        m_Window->Render();
        m_IsRunning = m_Window->IsOpen();
    }

    Cleanup();
}

void ClientApp::Update()
{
    if (m_PlayerTurnDelay > sf::Time::Zero)
    {
        m_PlayerTurnDelay -= delta;
        return;
    }

    CheckIfMouseHoverBoard();
}


void ClientApp::CheckIfMouseHoverBoard()
{
    for (size_t i = 0; i < m_Board->GetTotalSize(); i++)
    {
        if (IsMouseHoverPiece(i))
        {
            if (m_Window->IsMouseButtonPressed(sf::Mouse::Left))
            {
                m_Window->UnregisterDrawable(m_Board->GetPieceAt(i).GetShape());
                m_Board->GetPieceAt(i).SetPlayerPiece(m_IsPlayerOneTurn ? m_PlayerOne : m_PlayerTwo);
                m_Window->RegisterDrawable(m_Board->GetPieceAt(i).GetShape());

                m_IsGameFinished = m_Board->IsThereAWinner();
            }
        }
    }
}

bool ClientApp::IsMouseHoverPiece(size_t i) const
{
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(m_Window->GetMousePosition());
    size_t size = m_Board->GetPieceSize();

    return  mousePos.x > m_Board->GetPieceAt(i).GetPosition().x &&
			mousePos.x < m_Board->GetPieceAt(i).GetPosition().x + size &&
			mousePos.y > m_Board->GetPieceAt(i).GetPosition().y &&
			mousePos.y < m_Board->GetPieceAt(i).GetPosition().y + size;
}

void ClientApp::Cleanup()
{
    RELEASE(m_PlayerOne);
    RELEASE(m_PlayerTwo);
    RELEASE(m_Board);

    for (auto& drawable : m_Window->GetDrawables())
    {
        RELEASE(drawable);
    }

    RELEASE(m_Window);
}

#include "ClientApp.h"
#include "Player.h"
#include "TicTacToe.h"
#include "Window.h"
#include "src/core/PlayerPiece.h"

using namespace TicTacToe;

void ClientApp::Init()
{
    m_IsRunning = true;
    m_Window = new Window();
    m_Window->Create("Tic Tac Toe Online!", 1280, 720);
    
    std::cout << "Hello World! I'm a client!\n";

    m_Board = new Board();

    m_PlayerOne = new Player("Player One");
    m_PlayerTwo = new Player("Player Two");

    DrawBoard();
}

void ClientApp::DrawBoard()
{
    const int pieceSize = m_Board.GetPieceSize();
    const int width = m_Board.GetWidth();
    const int height = m_Board.GetHeight();
    const sf::Vector2f center = m_Window->GetCenter();

    // Draw the board - temp
    for (size_t i = 0; i < m_Board.GetTotalSize(); ++i)
    {
        auto* square = new sf::RectangleShape(sf::Vector2f(pieceSize, pieceSize));
        square->setFillColor(sf::Color::Color(51, 56, 63));
        square->setOutlineColor(sf::Color::Color(0, 189, 156));
        square->setOutlineThickness(OUTLINE_THICKNESS);
        square->setPosition(center.x - (width * pieceSize * 0.5f) + (i % width) * pieceSize + OUTLINE_THICKNESS * (i % width),
                            center.y - (height * pieceSize * 0.5f) + (i / height) * pieceSize + OUTLINE_THICKNESS * (i / height));

        m_Window->RegisterDrawable(square);
        m_Board.GetGraphicPiece(i).SetShape(square);
        m_Board.GetGraphicPiece(i).SetPosition(square->getPosition());
    }
}

void ClientApp::Run()
{
    if (!m_IsRunning)
        throw std::runtime_error("ClientApp is not initialized!");

    sf::Clock clock;

    while (m_IsRunning)
    {
        const sf::Time elapsed = clock.restart();

        m_Window->PollEvents();
        Update(elapsed);
        m_Window->Render();
        m_IsRunning = m_Window->IsOpen();
    }

    Cleanup();
}

void ClientApp::Update(sf::Time delta)
{
    if (m_PlayerTurnTimer > sf::Time::Zero)
    {
        m_PlayerTurnTimer -= delta;
        return;
    }

    CheckIfMouseHoverBoard();
}


void ClientApp::CheckIfMouseHoverBoard()
{
    for (size_t i = 0; i < m_Board.GetTotalSize(); i++)
    {
        if (m_Board[i].GetPlayerID() != EMPTY_PIECE) continue;

        if (IsMouseHoverPiece(i))
        {
            if (m_Window->IsMouseButtonPressed(sf::Mouse::Left))
            {
                PlacePlayerPieceOnBoard(i);

                const int winnerID = m_Board.IsThereAWinner();
                if (winnerID != EMPTY_PIECE)
                {
                    std::cout << "Player " << winnerID << " won!\n";
                }

                if (m_Board.IsFull() || winnerID != EMPTY_PIECE)
                    ClearBoard();

                SwitchPlayerTurn();
            }
        }
    }
}

void ClientApp::PlacePlayerPieceOnBoard(size_t i)
{
    m_Board[i].SetPlayerPiece(m_IsPlayerOneTurn ? &m_PlayerOne : &m_PlayerTwo);

    auto pos = sf::Vector2f( m_Board.GetGraphicPiece(i).GetPosition());

    // Center the piece
    pos.x += static_cast<float>(m_Board.GetPieceSize()) * 0.5f;
    pos.y += static_cast<float>(m_Board.GetPieceSize()) * 0.5f;

    if (m_IsPlayerOneTurn)
    {
        auto* piece = new PlayerCircleShape(&m_PlayerOne);
        piece->setPosition(pos);
        m_Window->RegisterDrawable(piece);
        m_GamePieces.push_back(piece);
    }
    else
    {
        auto* piece = new PlayerCrossShape(&m_PlayerTwo);
        piece->setPosition(pos);
        m_Window->RegisterDrawable(piece);
        m_GamePieces.push_back(piece);
    }
}

void ClientApp::ClearBoard()
{
    for (auto& piece : m_GamePieces)
    {
        m_Window->UnregisterDrawable(piece);
        RELEASE(piece);
    }

    m_GamePieces.clear();
    m_Board.Clear();
}

void ClientApp::SwitchPlayerTurn()
{
    m_IsPlayerOneTurn = !m_IsPlayerOneTurn;
    m_PlayerTurnTimer = sf::seconds(PLAYER_TURN_DELAY);
}


bool ClientApp::IsMouseHoverPiece(size_t i)
{
	const sf::Vector2f mousePos = static_cast<sf::Vector2f>(m_Window->GetMousePosition());
    int size = m_Board.GetPieceSize();
    const sf::Vector2f piecePosition = m_Board.GetGraphicPiece(i).GetPosition();

    return  mousePos.x > piecePosition.x &&
			mousePos.x < piecePosition.x + size &&
			mousePos.y > piecePosition.y &&
			mousePos.y < piecePosition.y + size;
}

void ClientApp::Cleanup()
{
    for (auto& drawable : m_Window->GetDrawables())
    {
        RELEASE(drawable);
    }

    RELEASE(m_Window);
}

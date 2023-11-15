#include "ClientApp.h"

#include <assert.h>

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

    m_Board.Init();
    m_ScoreManager.Init();
    m_PlayerManager.Init();

    sf::Vector2f center = m_Window->GetCenter();

    const auto text = new sf::Text();
    text->setFont(m_Font);
    text->setString("Tic Tac Toz");
    text->setCharacterSize(48);
    text->setFillColor(sf::Color::White);
    text->setStyle(sf::Text::Bold | sf::Text::Underlined);
    text->setPosition(center.x - text->getGlobalBounds().width * 0.5f + 20, 0.0f + 20);

    m_Window->RegisterDrawable(text);

    m_PlayerManager.CreateNewPlayer("Player One");
    m_PlayerManager.CreateNewPlayer("Player Two");

    for (auto& player : m_PlayerManager.GetAllPlayers())
    {
        m_ScoreManager.CreateScoreForPlayer(player->GetPlayerData(), m_Window);
    }

    m_PlayerTurnText = new sf::Text();
    m_PlayerTurnText->setFont(m_Font);
    m_PlayerTurnText->setString(m_PlayerManager.GetCurrentPlayer()->GetName() + " turn");
    m_PlayerTurnText->setCharacterSize(24);
    // TODO : Change color based on player turn
    m_PlayerTurnText->setFillColor(sf::Color::White);
    m_PlayerTurnText->setStyle(sf::Text::Bold);
    m_PlayerTurnText->setPosition(m_Window->GetWidth() - m_PlayerTurnText->getGlobalBounds().width - 75, m_Window->GetHeight() * 0.5f - m_PlayerTurnText->getGlobalBounds().height);

    m_Window->RegisterDrawable(m_PlayerTurnText);

    m_GameStateText = new sf::Text();
    m_GameStateText->setFont(m_Font);
    m_GameStateText->setString("");
    m_GameStateText->setCharacterSize(24);
    // TODO : Change color based on player turn
    m_GameStateText->setFillColor(sf::Color::White);
    m_GameStateText->setStyle(sf::Text::Bold);
    m_GameStateText->setPosition(m_PlayerTurnText->getPosition().x, 100);
   
    m_Window->RegisterDrawable(m_GameStateText);

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
                m_GameStateText->setString("");

                PlacePlayerPieceOnBoard(i);

                const int winnerID = m_Board.IsThereAWinner();
                if (winnerID != EMPTY_PIECE)
                {
                    std::cout << "Player " << winnerID << " won!\n";

                    Player* winner = m_PlayerManager.GetCurrentPlayer();

                    m_ScoreManager.SaveGame(winner->GetPlayerData());
                    m_ScoreManager.AddScoreToPlayer(winner->GetPlayerData());

                    m_GameStateText->setString(winner->GetName() + " won!");

                    ClearBoard();
                }
                else if (m_Board.IsFull())
                {
                    m_GameStateText->setString("It's a draw!");
                    ClearBoard();
                }

                SwitchPlayerTurn();
            }
        }
    }
}

void ClientApp::PlacePlayerPieceOnBoard(size_t i)
{
    Player* currentPlayer = m_PlayerManager.GetCurrentPlayer();
    m_Board[i].SetPlayerPiece(currentPlayer);

    auto pos = sf::Vector2f( m_Board.GetGraphicPiece(i).GetPosition());

    // Center the piece
    pos.x += static_cast<float>(m_Board.GetPieceSize()) * 0.5f;
    pos.y += static_cast<float>(m_Board.GetPieceSize()) * 0.5f;

    auto piece = new GraphicPiece(currentPlayer->GetGraphicPiece());

    if (m_IsPlayerOneTurn)
    {
        auto piece = new PlayerCircleShape(currentPlayer);
        piece->setPosition(pos);
        m_Window->RegisterDrawable(piece);
        m_GamePieces.push_back(piece);
    }
    else
    {
        auto* piece = new PlayerCrossShape(currentPlayer);
        piece->setPosition(pos);
        m_Window->RegisterDrawable(piece);
        m_GamePieces.push_back(piece);
    }

    m_ScoreManager.AddPlayerMove(currentPlayer->GetPlayerID(), i);
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
    m_PlayerManager.SwitchPlayerTurn();

    m_PlayerTurnTimer = sf::seconds(PLAYER_TURN_DELAY);
    m_PlayerTurnText->setString(m_PlayerManager.GetCurrentPlayer()->GetName() + " turn");

    // TODO : Change color based on player turn
    if (m_IsPlayerOneTurn)
        m_PlayerTurnText->setFillColor(sf::Color::Color(250, 92, 12));
    else 
        m_PlayerTurnText->setFillColor(sf::Color::Color(255, 194, 0));
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

    NULLPTR(m_PlayerTurnText);
    NULLPTR(m_GameStateText);

    RELEASE(m_Window);
}

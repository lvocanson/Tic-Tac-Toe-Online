#include "ClientApp.h"
#include "Player.h"
#include "TicTacToe.h"
#include "Window.h"
#include "src/tcp-ip/TcpIpClient.h"
#include "src/core/PlayerPiece.h"

using namespace TicTacToe;

void ClientApp::Init()
{
    m_IsRunning = true;
    m_Window = new Window();
    m_Window->Create("Tic Tac Toe Online!", 1280, 720);
    
    std::cout << "Hello World! I'm a client!\n";

    m_Board.Init();

    m_PlayerOne.SetName("Player One");
    m_PlayerTwo.SetName("Player Two");

    DrawBoard();
}

void ClientApp::DrawBoard()
{
    const float pieceSize = m_Board.GetPieceSize();
    const size_t width = m_Board.GetWidth();
    const size_t height = m_Board.GetHeight();
    const sf::Vector2f center = m_Window->GetCenter();

    // Draw the board - temp
    for (unsigned int i = 0; i < m_Board.GetTotalSize(); ++i)
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

    auto& client = TcpIpClient::GetInstance();
    try
    {
        client.Connect("localhost", DEFAULT_PORT);
        DebugLog("Connected to server!\n");
        client.Send("Hello from client!");
    }
    catch (const TcpIp::TcpIpException& e)
    {
        DebugLog("Failed to connect to server: " + std::string(e.what()) + "\n");
        m_IsRunning = false;
    }

    std::stringstream ss;
    sf::Clock clock;

    while (m_IsRunning)
    {
        const sf::Time elapsed = clock.restart();

        m_Window->PollEvents();
        Update(elapsed);
        m_Window->Render();
        m_IsRunning = m_Window->IsOpen();

        try
        {
            while (client.FetchPendingData(ss))
            {
                DebugLog("Received data from server: \n");
                DebugLog(ss.str().c_str());
                DebugLog("\n");
                ss.str(std::string());
            }
        }
        catch (const TcpIp::TcpIpException& e)
        {
            DebugLog("Failed to fetch data from server: " + std::string(e.what()) + "\n");
            m_IsRunning = false;
        }

        if (!client.IsConnected())
        {
            DebugLog("Disconnected from server!\n");
            m_IsRunning = false;
        }
    }

    client.Disconnect();
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
    for (unsigned int i = 0; i < m_Board.GetTotalSize(); i++)
    {
        if (m_Board[i] != EMPTY_PIECE) continue;

        if (IsMouseHoverPiece(i))
        {
            if (m_Window->IsMouseButtonPressed(sf::Mouse::Left))
            {
                PlacePlayerPieceOnBoard(i);

                const int winnerID = m_Board.IsThereAWinner();
                if (winnerID != EMPTY_PIECE)
                {
                    std::cout << "Player " << winnerID << " won!\n";
                    TcpIpClient::GetInstance().Send("A player won ! " + winnerID);
                }

                if (m_Board.IsFull() || winnerID != EMPTY_PIECE)
                {
                    ClearBoard();
                    TcpIpClient::GetInstance().Send("It's a draw !");
                }

                SwitchPlayerTurn();
            }
        }
    }
}

void ClientApp::PlacePlayerPieceOnBoard(unsigned int i)
{
    m_Board[i] = (m_IsPlayerOneTurn ? m_PlayerOne.GetPlayerID() : m_PlayerTwo.GetPlayerID());

    int row = i / m_Board.GetWidth();
    int col = i % m_Board.GetWidth();
    std::string playerID = std::to_string(m_Board[i]);
    TcpIpClient::GetInstance().Send("A piece has been placed at row: " + std::to_string(row) + "||col: " + std::to_string(col) + " by player " + playerID);

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
    m_Board.SetEmpty();
}

void ClientApp::SwitchPlayerTurn()
{
    m_IsPlayerOneTurn = !m_IsPlayerOneTurn;
    m_PlayerTurnTimer = sf::seconds(PLAYER_TURN_DELAY);
    if(m_IsPlayerOneTurn)
        TcpIpClient::GetInstance().Send(m_PlayerOne.GetName() + " Turn");
    else
        TcpIpClient::GetInstance().Send(m_PlayerTwo.GetName() + " Turn");
}


bool ClientApp::IsMouseHoverPiece(unsigned int i)
{
	const sf::Vector2f mousePos = static_cast<sf::Vector2f>(m_Window->GetMousePosition());
    const float size = m_Board.GetPieceSize();
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

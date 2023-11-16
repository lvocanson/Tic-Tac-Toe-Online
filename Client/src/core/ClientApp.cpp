#include "ClientApp.h"

#include "Player.h"
#include "TicTacToe.h"
#include "Window.h"
#include "src/tcp-ip/TcpIpClient.h"
#include "src/core/PlayerPieceShape.h"

using namespace TicTacToe;

void ClientApp::Init()
{
    m_IsRunning = true;
    m_Window = new Window();
    m_Window->Create("Tic Tac Toe Online!", 1280, 720);

    m_GameStateUI = new GameStateUI(m_Window);

    std::cout << "Hello World! I'm a client!\n";

    m_Board.Init();
    m_ScoreManager.Init();
    m_PlayerManager.Init();

    m_PlayerManager.CreateNewPlayer("Player One", sf::Color(250, 92, 12), Circle);
    m_PlayerManager.CreateNewPlayer("Player Two", sf::Color(255, 194, 0), Square);

    m_GameStateUI->Init();

    for (const auto& player : m_PlayerManager.GetAllPlayers())
    {
        m_ScoreManager.CreateScoreForPlayer(player->GetData(), m_Window);
    }

    m_GameStateUI->InitPlayerScores(m_PlayerManager.GetAllPlayers());

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
        m_InputHandler.Update();

        Update(elapsed);
        m_Window->Render();
        m_IsRunning = m_Window->IsOpen();

        try
        {
            if (client.FetchPendingData(ss))
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
    for (unsigned int cell = 0; cell < m_Board.GetTotalSize(); cell++)
    {
        if (m_Board[cell] != EMPTY_PIECE) continue;

        if (IsMouseHoverPiece(cell))
        {
            bool tst = InputHandler::IsMouseButtonPressed(sf::Mouse::Left);
            if (InputHandler::IsMouseButtonPressed(sf::Mouse::Left))
            {
                m_GameStateUI->UpdateGameStateText("");

                PlacePlayerPieceOnBoard(cell);

                const PieceID winnerID = m_Board.IsThereAWinner();
                if (winnerID != EMPTY_PIECE)
                {
                    DebugLog("Player " + std::to_string(winnerID) + " won!\n");

                    Player* winner = PlayerManager::GetCurrentPlayer();

                    m_ScoreManager.AddScoreToPlayer(winner->GetPlayerID());
                    m_ScoreManager.SaveGame(winner->GetData());

                    m_GameStateUI->UpdatePlayerScore(*winner->GetData(), m_ScoreManager.GetPlayerScore(winner->GetPlayerID()));
                    m_GameStateUI->UpdateGameStateText(winner->GetName() + " won!");

                    ClearBoard();
                }
                else if (m_Board.IsFull())
                {
                    m_GameStateUI->UpdateGameStateText("It's a draw!");
                    ClearBoard();
                }

                SwitchPlayerTurn();
            }
        }
    }
}

void ClientApp::PlacePlayerPieceOnBoard(unsigned int cell)
{
    const Player* currentPlayer = PlayerManager::GetCurrentPlayer();

    // Set piece id in board
    m_Board[cell] = currentPlayer->GetPlayerID();

    InstanciateNewPlayerShape(currentPlayer, cell);

    m_ScoreManager.AddPlayerMove(currentPlayer->GetPlayerID(), cell);
}

void ClientApp::InstanciateNewPlayerShape(const Player* currentPlayer, unsigned int cell)
{
    auto pos = sf::Vector2f(m_Board.GetGraphicPiece(cell).GetPosition());

    // Center the piece
    pos.x += m_Board.GetPieceSize() * 0.5f;
    pos.y += m_Board.GetPieceSize() * 0.5f;

    auto playerPieceShape = new PlayerPieceShape(currentPlayer->GetPlayerID(), pos);
    m_GamePieces.push_back(playerPieceShape);
    m_Window->RegisterDrawable(playerPieceShape);
}

void ClientApp::SwitchPlayerTurn()
{
    m_PlayerManager.SwitchPlayerTurn();

    m_PlayerTurnTimer = sf::seconds(PLAYER_TURN_DELAY);

    m_GameStateUI->UpdatePlayerTurnText(*PlayerManager::GetCurrentPlayer()->GetData());
}


bool ClientApp::IsMouseHoverPiece(unsigned int i)
{
	const sf::Vector2f mousePos = static_cast<sf::Vector2f>(InputHandler::GetMousePosition());
    const float size = m_Board.GetPieceSize();
    const sf::Vector2f piecePosition = m_Board.GetGraphicPiece(i).GetPosition();

    return  mousePos.x > piecePosition.x &&
			mousePos.x < piecePosition.x + size &&
			mousePos.y > piecePosition.y &&
			mousePos.y < piecePosition.y + size;
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

void ClientApp::Cleanup()
{
    for (auto& drawable : m_Window->GetDrawables())
    {
        RELEASE(drawable);
    }

    RELEASE(m_Window);
    RELEASE(m_GameStateUI);

    FontRegistry::ClearFonts();
}

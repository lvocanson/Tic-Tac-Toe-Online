#include "TicTacToe.h"

#include <iostream>


#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
// Dynamically allocate memory with memory leak detection. (debug only)
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

namespace TicTacToe
{
	Board::Board(size_t width, size_t height, int pieceSize) : width(width), height(height), size(width * height), pieceSize(pieceSize)
	{
		for (size_t i = 0; i < size; ++i)
		{
			m_board.push_back(new Piece());
		}
	}

	Board::~Board()
	{
	    for (auto& piece : m_board)
	    {
		    RELEASE(piece);
	    }
	}

	bool Board::IsFull() const
	{
	    for (int i = 0; i < size; i++)
	    {
	        if (m_board[i]->GetPlayerID() == EMPTY_PIECE)
	        {
	            return false;
	        }
	    }
	    return true;
	}

	void Board::Clear()
	{
	    for (int i = 0; i < size; i++)
	    {
			m_board[i]->Clear();
	    }
	}

	bool Board::IsThereAWinner() const
	{
		for (int i = 0; i < size; i++)
		{
			if (m_board[i]->GetPlayerID() == EMPTY_PIECE)
			{
				continue;
			}

			// Check for horizontal win
			if (i % width == 0)
			{
				for (int j = 0; j < width; j++)
				{
					if (m_board[i + j]->GetPlayerID() != m_board[i]->GetPlayerID())
					{
						break;
					}
					if (j == width - 1)
					{
						// We have a winner!
						std::cout << "Player " << m_board[i]->GetPlayerID() << " has won!\n";
						return true;
					}
				}
			}

			// Check for vertical win
			if (i < width)
			{
				for (int j = 0; j < size; j += width)
				{
					if (m_board[i + j]->GetPlayerID() != m_board[i]->GetPlayerID())
					{
						break;
					}
					if (j == size - width)
					{
												// We have a winner!
						std::cout << "Player " << m_board[i]->GetPlayerID() << " has won!\n";
						return true;
					}
				}
			}

			// Check for top-left diagonal win
			if (i == 0)
			{
				for (int j = 0; j < size; j += width + 1)
				{
					if (m_board[i + j]->GetPlayerID() != m_board[i]->GetPlayerID())
					{
						break;
					}
					if (j == size - 1)
					{
												// We have a winner!
						std::cout << "Player " << m_board[i]->GetPlayerID() << " has won!\n";
						return true;
					}
				}
			}

			// Check  for top-right diagonal win
			if (i == width - 1)
			{
				for (int j = 0; j < size - 1; j += width - 1)
				{
					if (m_board[i + j]->GetPlayerID() != m_board[i]->GetPlayerID())
					{
						break;
					}
					if (i + j == size - width)
					{
						// We have a winner!
						std::cout << "Player " << m_board[i]->GetPlayerID() << " has won!\n";
						return true;
					}
				}
			}
		}

		return false;
	}

	Piece::Piece() : m_PlayerID(EMPTY_PIECE)
	{

	}

	Piece::Piece(sf::Vector2f position) : m_PlayerID(EMPTY_PIECE)
	{

    }

	Piece::~Piece()
	{
		Clear();
	}

	void Piece::SetPlayerPiece(const Player* player)
	{
		m_PlayerID = player->GetPlayerID();
	}

	void Piece::Clear()
	{
		m_PlayerID = EMPTY_PIECE;
	}

}

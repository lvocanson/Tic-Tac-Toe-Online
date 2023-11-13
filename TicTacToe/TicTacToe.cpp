#include "TicTacToe.h"

#include <iostream>


#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
// Dynamically allocate memory with memory leak detection. (debug only)
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

namespace TicTacToe
{
	Board::Board(size_t width, size_t height, int pieceSize, int winnablePieces) : m_Width(width), m_Height(height), m_Size(width * height), m_PieceSize(pieceSize), m_WinnablePieces(winnablePieces)
	{
		for (size_t i = 0; i < m_Size; ++i)
		{
			m_Board.push_back(new Piece());
		}
	}

	Board::~Board()
	{
	    for (auto& piece : m_Board)
	    {
			if (piece)
			{
				delete piece;
				piece = nullptr;
			}
	    }
	}

	bool Board::IsFull() const
	{
	    for (int i = 0; i < m_Size; i++)
	    {
	        if (m_Board[i]->GetPlayerID() == EMPTY_PIECE)
	        {
	            return false;
	        }
	    }
	    return true;
	}

	void Board::Clear()
	{
	    for (int i = 0; i < m_Size; i++)
	    {
			m_Board[i]->Clear();
	    }
	}

	int Board::IsThereAWinner() const
	{
		int winner = EMPTY_PIECE;
		int continuousPiecesCount = 0;

		for (int i = 0; i < m_Size; i++)
		{
			if (m_Board[i]->GetPlayerID() == EMPTY_PIECE)
			{
				continue;
			}

			// Check for horizontal win
			if (i % m_Width == 0)
			{
				continuousPiecesCount = 0;
				for (int j = 0; j < m_Width; j++)
				{
					if (m_Board[i + j]->GetPlayerID() != m_Board[i]->GetPlayerID())
					{
						break;
					}
					else 
					{
						continuousPiecesCount++;
						if (continuousPiecesCount >= m_WinnablePieces)
						{
						    return m_Board[i]->GetPlayerID();
                        }
					}
				}
			}

			// Check for vertical win
			if (i < m_Width)
			{
				continuousPiecesCount = 0;

				for (int j = 0; j < m_Size; j += m_Width)
				{
					if (m_Board[i + j]->GetPlayerID() != m_Board[i]->GetPlayerID())
					{
						break;
					}
					else
					{
						continuousPiecesCount++;
						if (continuousPiecesCount == m_WinnablePieces)
						{
							return m_Board[i]->GetPlayerID();
						}
					}
				}
			}

			// Check for top-left diagonal win
			if (i == 0)
			{
				continuousPiecesCount = 0;

				for (int j = 0; j < m_Size; j += m_Width + 1)
				{
					if (m_Board[i + j]->GetPlayerID() != m_Board[i]->GetPlayerID())
					{
						break;
					}
					else
					{
						continuousPiecesCount++;
						if (continuousPiecesCount == m_WinnablePieces)
						{
							return m_Board[i]->GetPlayerID();
						}
					}
				}
			}

			// Check  for top-right diagonal win
			if (i == m_Width - 1)
			{
				continuousPiecesCount = 0;

				for (int j = 0; j < m_Size - 1; j += m_Width - 1)
				{
					if (m_Board[i + j]->GetPlayerID() != m_Board[i]->GetPlayerID())
					{
						break;
					}
					else
					{
						continuousPiecesCount++;
						if (continuousPiecesCount == m_WinnablePieces)
						{
							return m_Board[i]->GetPlayerID();
						}
					}
				}
			}
		}

		return EMPTY_PIECE;
	}

	Piece::Piece() : m_PlayerID(EMPTY_PIECE)
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

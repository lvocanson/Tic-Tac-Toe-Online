#include "TicTacToe.h"
#include <iostream>
#include <vector>


#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
// Dynamically allocate memory with memory leak detection. (debug only)
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

namespace TicTacToe
{
	Board::Board(size_t width, size_t height, unsigned int alignementGoal)
		: m_Width(width)
		, m_Height(height)
		, m_Size(width* height)
		, m_AlignementGoal(alignementGoal)
		, m_Board(new PieceID[m_Size])
	{
        SetEmpty();
	}

	Board::~Board()
	{
	    delete[] m_Board;
		m_Board = nullptr;
	}

	bool Board::IsFull() const
	{
	    for (int i = 0; i < m_Size; i++)
	    {
	        if (m_Board[i] == EMPTY_PIECE)
	        {
	            return false;
	        }
	    }
	    return true;
	}

	// TODO: Needs to be reworked cauz it doesn't work if we change the grid size and the win condition
	PieceID Board::IsThereAWinner() const
	{
		PieceID winner = EMPTY_PIECE;
		unsigned int continuousPiecesCount = 0;

		for (int i = 0; i < m_Size; i++)
		{
			if (m_Board[i] == EMPTY_PIECE)
			{
				continue;
			}

			// Check for horizontal win
			if (i % m_Width == 0)
			{
				continuousPiecesCount = 0;
				for (int j = 0; j < m_Width; j++)
				{
					if (m_Board[i + j] != m_Board[i])
					{
						break;
					}
					else
					{
						continuousPiecesCount++;
						if (continuousPiecesCount >= m_AlignementGoal)
						{
							return m_Board[i];
						}
					}
				}
			}

			// Check for vertical win
			if (i < m_Width)
			{
				continuousPiecesCount = 0;

				for (size_t j = 0; j < m_Size; j += m_Width)
				{
					if (m_Board[i + j] != m_Board[i])
					{
						break;
					}
					else
					{
						continuousPiecesCount++;
						if (continuousPiecesCount == m_AlignementGoal)
						{
							return m_Board[i];
						}
					}
				}
			}

			// Check for top-left diagonal win
			if (i == 0)
			{
				continuousPiecesCount = 0;

				for (size_t j = 0; j < m_Size; j += m_Width + 1)
				{
					if (m_Board[i + j] != m_Board[i])
					{
						break;
					}
					else
					{
						continuousPiecesCount++;
						if (continuousPiecesCount == m_AlignementGoal)
						{
							return m_Board[i];
						}
					}
				}
			}

			// Check  for top-right diagonal win
			if (i == m_Width - 1)
			{
				continuousPiecesCount = 0;

				for (size_t j = 0; j < m_Size - 1; j += m_Width - 1)
				{
					if (m_Board[i + j] != m_Board[i])
					{
						break;
					}
					else
					{
						continuousPiecesCount++;
						if (continuousPiecesCount == m_AlignementGoal)
						{
							return m_Board[i];
						}
					}
				}
			}
		}

		return EMPTY_PIECE;
	}

	unsigned int Board::GetRandomEmptyCell() const
	{
		srand(static_cast<unsigned>(time(nullptr)));

		std::vector<unsigned int> emptyCells;

		for (unsigned int i = 0; i < m_Size; i++)
		{
		    if (m_Board[i] == EMPTY_PIECE)
		    {
		        emptyCells.push_back(i);
            }
        }

		return emptyCells[rand() % emptyCells.size()];
	}

	void Board::Resize(size_t width, size_t height)
	{
		m_Width = width;
		m_Height = height;
		m_Size = width * height;
		delete[] m_Board;
		m_Board = new PieceID[m_Size];
		SetEmpty();
	}

	void Board::SetEmpty()
	{
	    for (int i = 0; i < m_Size; i++)
	    {
			m_Board[i] = EMPTY_PIECE;
	    }
	}
}

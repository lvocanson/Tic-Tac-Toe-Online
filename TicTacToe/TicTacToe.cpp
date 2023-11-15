#include "TicTacToe.h"
#include "Player.h"
#include <iostream>


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
		, m_Board(new Piece[m_Size])
	{
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
	        if (m_Board[i].GetPlayerID() == EMPTY_PIECE)
	        {
	            return false;
	        }
	    }
	    return true;
	}

	Piece& Board::GetWinner() const
	{
		// TODO: Implement
		return m_Board[0];
	}

	void Board::Resize(size_t width, size_t height)
	{
		m_Width = width;
		m_Height = height;
		m_Size = width * height;
		delete[] m_Board;
		m_Board = new Piece[m_Size];
		SetEmpty();
	}

	void Board::SetEmpty()
	{
	    for (int i = 0; i < m_Size; i++)
	    {
			m_Board[i].SetEmpty();
	    }
	}
}

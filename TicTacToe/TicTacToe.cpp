#include "TicTacToe.h"

#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
// Dynamically allocate memory with memory leak detection. (debug only)
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

namespace TicTacToe
{
Board::Board(size_t width, size_t height) : width(width), height(height), size(width * height)
{
    m_board = new Piece[size];
    Clear();
}

Board::~Board()
{
    delete[] m_board;
    m_board = nullptr;
}

bool Board::IsFull() const
{
    for (int i = 0; i < size; i++)
    {
        if (m_board[i] == Piece::None)
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
        m_board[i] = Piece::None;
    }
}
}

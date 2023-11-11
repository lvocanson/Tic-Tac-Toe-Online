#include "TicTacToe.h"

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

#pragma once
#include <vector>
#include "Player.h"

constexpr int DEFAULT_WINNABLE_PIECES = 3;


namespace TicTacToe
{

    /// <summary>
    /// Represents a piece on the board.
    /// </summary>
    typedef unsigned int Piece;
    constexpr const Piece EMPTY_PIECE = 0;

    /// <summary>
    /// Represents a board of Tic-Tac-Toe.
    /// </summary>
    class Board
    {
    public:
        /// <summary>
        /// Creates a 3x3 board with 3 pieces in a row needed to win.
        /// </summary>
        Board() : Board(DEFAULT_BOARD_ARGS) {}
        /// <summary>
        /// Creates a board with the specified width and height.
        /// </summary>
        Board(size_t width, size_t height, unsigned int ²);
        virtual ~Board();

        size_t GetWidth() const { return m_Width; }
        size_t GetHeight() const { return m_Height; }
        size_t GetTotalSize() const { return m_Size; }

        /// <summary>
        /// Returns a reference to the piece at the specified row and column.
        /// </summary>
        Piece& operator()(size_t row, size_t col) { return m_Board[row * m_Width + col]; }
        /// <summary>
        /// Returns a const reference to the piece at the specified row and column.
        /// </summary>
        const Piece& operator()(size_t row, size_t col) const { return m_Board[row * m_Width + col]; }
        /// <summary>
        /// Returns a reference to the piece at the specified index.
        /// </summary>
        Piece& operator[](size_t index) { return m_Board[index]; }
        /// <summary>
        /// Returns a const reference to the piece at the specified index.
        /// </summary>
        const Piece& operator[](size_t index) const { return m_Board[index]; }

        /// <summary>
        /// Returns true if the board does not contain any empty pieces.
        /// </summary>
        bool IsFull() const;

        /// <summary>
        /// Returns the winning piece, or the empty piece if there is no winner.
        /// </summary>
        Piece IsThereAWinner() const;

        /// <summary>
        /// Resizes the board to the specified width and height.
        /// </summary>
        void Resize(size_t width, size_t height);

        /// <summary>
        /// Sets all the pieces on the board to empty pieces.
        /// </summary>
        void SetEmpty();

    protected:

        size_t m_Width = 3, m_Height = 3, m_Size = 9;
        unsigned int m_AlignementGoal = 3;
        Piece* m_Board;
    };
}

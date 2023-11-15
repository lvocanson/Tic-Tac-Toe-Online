#pragma once
#include <vector>
#include "Player.h"

constexpr int DEFAULT_WINNABLE_PIECES = 3;


namespace TicTacToe
{
	/// <summary>
	/// Represents a piece on the board.
	/// </summary>
	class Piece
	{
	public:
		Piece();
		virtual ~Piece();

		int GetPlayerID() const { return m_PlayerID; }

		/// <summary>
		/// Sets the id of the player that owns this piece
		/// </summary>
		void SetPlayerPiece(const Player* player);

		virtual void Clear();

	private:

		int m_PlayerID = EMPTY_PIECE;

	};

	/// <summary>
	/// Represents a board of Tic-Tac-Toe.
	/// </summary>
	class Board
	{
	public:
	    /// <summary>
	    /// Creates a 3x3 board and sets the size of each piece to 100.
	    /// </summary>
	    Board() : Board(3, 3, DEFAULT_WINNABLE_PIECES) {}
	    /// <summary>
	    /// Creates a board with the specified width and height.
	    /// </summary>
	    Board(size_t width, size_t height, int winnablePieces);
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
	    /// Returns true if the board does not contain a piece with EMPTY_PIECE id
	    /// </summary>
	    bool IsFull() const;

	    /// <summary>
	    /// Sets all the pieces on the board to EMPTY_PIECE id and reset their shape
	    /// </summary>
	    void Clear();

		/// <summary>
		/// Checks if there is a winner on the board and return his id
		/// </summary>
	    int IsThereAWinner() const;

    protected:

		virtual void Init();

	    size_t m_Width = 3, m_Height = 3, m_Size = 9;
		int m_WinnablePieces = 3;
	    Piece* m_Board;

	};
}

#pragma once
#include <string>

#include "Player.h"

#include "../Client/src/core/CoreDefinitions.h"


namespace TicTacToe
{
	/// <summary>
	/// Represents a piece on the board.
	/// </summary>
	class Piece
	{
	public:
		Piece();
		Piece(sf::Vector2f position);
		~Piece();

		sf::Shape* GetShape() const { return m_Shape; }
		int GetPlayerID() const { return m_PlayerID; }
		sf::Vector2f GetPosition() const { return m_Shape->getPosition(); }

		/// <summary>
		/// Sets the id of the player that owns this piece and changes the shape of the piece to match the player's shape.
		/// </summary>
		void SetPlayerPiece(const Player* player);
		void SetPosition(sf::Vector2f position) const { m_Shape->setPosition(position); }

		void Clear();

	private:

		sf::Shape* m_Shape = nullptr;
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
	    Board() : Board(3, 3, 100) {}
	    /// <summary>
	    /// Creates a board with the specified width and height.
	    /// </summary>
	    Board(size_t width, size_t height, int pieceSize);
	    ~Board();

	    size_t GetWidth() const { return width; }
	    size_t GetHeight() const { return height; }
	    size_t GetTotalSize() const { return size; }
		size_t GetPieceSize() const { return pieceSize; }
		Piece& GetPieceAt(size_t index) const { return *m_board[index]; }
		Piece& GetPieceAt(size_t row, size_t col) const { return *m_board[row * width + col]; }


	    /// <summary>
	    /// Returns a reference to the piece at the specified row and column.
	    /// </summary>
	    Piece& operator()(size_t row, size_t col) { return *m_board[row * width + col]; }
	    /// <summary>
	    /// Returns a const reference to the piece at the specified row and column.
	    /// </summary>
	    const Piece& operator()(size_t row, size_t col) const { return *m_board[row * width + col]; }
	    /// <summary>
	    /// Returns a reference to the piece at the specified index.
	    /// </summary>
	    Piece& operator[](size_t index) { return *m_board[index]; }
	    /// <summary>
	    /// Returns a const reference to the piece at the specified index.
	    /// </summary>
	    const Piece& operator[](size_t index) const { return *m_board[index]; }

	    /// <summary>
	    /// Returns true if the board does not contain a piece with EMPTY_PIECE id
	    /// </summary>
	    bool IsFull() const;

	    /// <summary>
	    /// Sets all the pieces on the board to EMPTY_PIECE id and reset their shape
	    /// </summary>
	    void Clear();

		/// <summary>
		/// Checks if there is a winner on the board.
		/// </summary>
	    bool IsThereAWinner();

    protected:
	    size_t width = 3, height = 3, size = 9;
		int pieceSize = 100;
	    std::vector<Piece*> m_board;
	};
}

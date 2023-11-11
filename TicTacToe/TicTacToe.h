namespace TicTacToe
{
/// <summary>
/// Represents a piece on the board.
/// </summary>
enum class Piece : char
{
    None = ' ',
    X = 'X',
    O = 'O',
};

/// <summary>
/// Represents a board of Tic-Tac-Toe.
/// </summary>
class Board
{
public:
    /// <summary>
    /// Creates a 3x3 board.
    /// </summary>
    Board() : Board(3, 3) {}
    /// <summary>
    /// Creates a board with the specified width and height.
    /// </summary>
    Board(size_t width, size_t height);
    ~Board();

    size_t GetWidth() const { return width; }
    size_t GetHeight() const { return height; }
    size_t GetSize() const { return size; }

    /// <summary>
    /// Returns a reference to the piece at the specified row and column.
    /// </summary>
    Piece& operator()(size_t row, size_t col) { return m_board[row * width + col]; }
    /// <summary>
    /// Returns a const reference to the piece at the specified row and column.
    /// </summary>
    const Piece& operator()(size_t row, size_t col) const { return m_board[row * width + col]; }
    /// <summary>
    /// Returns a reference to the piece at the specified index.
    /// </summary>
    Piece& operator[](size_t index) { return m_board[index]; }
    /// <summary>
    /// Returns a const reference to the piece at the specified index.
    /// </summary>
    const Piece& operator[](size_t index) const { return m_board[index]; }

    /// <summary>
    /// Returns true if the board does not contain any Piece::None.
    /// </summary>
    bool IsFull() const;
    /// <summary>
    /// Sets all the pieces on the board to Piece::None.
    /// </summary>
    void Clear();

protected:
    size_t width = 3, height = 3, size = 9;
    Piece* m_board = nullptr;
};
}

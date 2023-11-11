#include "TicTacToe.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) && !defined(__CYGWIN__)

// Entry point for a Windows program (Unicode)
int wmain()
{
#if defined(DEBUG) | defined(_DEBUG)
    // Enable run-time memory check for debug builds.
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    std::cout << "Hello World! I'm a client!\n";

    using namespace TicTacToe;
    Board board;
    std::cout << "Created a " << board.GetWidth() << "x" << board.GetHeight() << " board (size " << board.GetSize() << ")\n";

    board(0, 1) = Piece::X;
    board(1, 1) = (Piece)'M';

    std::cout << "Is the board full? " << std::boolalpha << board.IsFull() << "\n";

    board(0, 0) = Piece::O;
    board(0, 2) = Piece::X;
    board(1, 0) = Piece::O;
    board(1, 2) = Piece::X;
    board(2, 0) = Piece::O;
    board(2, 1) = Piece::X;
    board(2, 2) = Piece::X;

    std::cout << "Is the board full? " << std::boolalpha << board.IsFull() << "\n";

    for (size_t i = 0; i < board.GetSize(); ++i)
    {
        std::cout << "board[" << i << "] = " << (char)board[i] << "\n";
    }

    return 0;
}

#else
#error Only Windows is supported
#endif

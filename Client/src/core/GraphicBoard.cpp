#include "src/pch.h"
#include "GraphicBoard.h"


GraphicBoard::GraphicBoard(size_t width, size_t height, unsigned int winnablePieces, float piecePixelSize) : Board(width, height, winnablePieces), m_PiecePixelSize(piecePixelSize)
{
}

GraphicBoard::~GraphicBoard()
{
    SetEmpty();

    for (auto& piece : m_AllPiecesOnBoard)
    {
        delete piece.second;
        piece.second = nullptr;
    }
}

void GraphicBoard::Init()
{
    for (int i = 0; i < m_Size; i++)
    {
        m_AllPiecesOnBoard.insert(std::pair(i, new GraphicPiece()));
    }
}

void GraphicBoard::AddPlayerPieceInBoard(unsigned int cell, const TicTacToe::Player* player, GraphicPiece* piece)
{
    m_AllPiecesOnBoard[cell] = piece;
    m_Board[cell].SetPlayerPiece(player);
}

void GraphicBoard::RemovePlayerPieceInBoard(unsigned int cell)
{
    m_AllPiecesOnBoard[cell] = nullptr;
    m_Board[cell].SetEmpty();
}

GraphicPiece::GraphicPiece() : m_Shape(nullptr)
{

}

GraphicPiece::~GraphicPiece()
{

}

void GraphicPiece::SetEmpty()
{
    Piece::SetEmpty();

    NULLPTR(m_Shape);
}

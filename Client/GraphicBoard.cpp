#include "src/pch.h"
#include "GraphicBoard.h"


GraphicBoard::GraphicBoard(size_t width, size_t height, int winnablePieces, int piecePixelSize) : Board(width, height, winnablePieces), m_PiecePixelSize(piecePixelSize)
{
    Init();
}

GraphicBoard::~GraphicBoard()
{

}

void GraphicBoard::Init()
{
    for (int i = 0; i < m_Size; i++)
    {
        m_Board.push_back(new GraphicPiece());
        m_AllPiecesOnBoard.insert(std::pair<int, GraphicPiece*>(i, nullptr));
    }
}

void GraphicBoard::AddPlayerPieceInBoard(int cell, GraphicPiece* piece)
{
    m_AllPiecesOnBoard[cell] = piece;
}

void GraphicBoard::RemovePlayerPieceInBoard(int cell)
{
    m_AllPiecesOnBoard[cell] = nullptr;
    m_Board[cell]->Clear();
}

GraphicPiece::GraphicPiece() : m_Position(0, 0), m_Shape(nullptr)
{

}

GraphicPiece::~GraphicPiece()
{

}

void GraphicPiece::Clear()
{
    Piece::Clear();

    NULLPTR(m_Shape);
}

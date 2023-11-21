#include "src/pch.h"
#include "src/core/Player.h"

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

void GraphicBoard::Init(unsigned int totalColumn, unsigned int totalRow)
{
    Resize(totalColumn, totalRow);

    for (unsigned int i = 0; i < m_Size; i++)
    {
        m_AllPiecesOnBoard.insert(std::pair(i, new GraphicPiece()));
    }
}

void GraphicBoard::AddPlayerPieceInBoard(unsigned int cell, const Player* player, GraphicPiece* piece)
{
    m_AllPiecesOnBoard[cell] = piece;
    m_Board[cell] = player->GetPlayerID();
}

void GraphicBoard::RemovePlayerPieceInBoard(unsigned int cell)
{
    m_AllPiecesOnBoard[cell]->SetEmpty();
    m_Board[cell] = TicTacToe::EMPTY_PIECE;
}

GraphicPiece::GraphicPiece() : m_Shape(nullptr)
{

}

GraphicPiece::~GraphicPiece()
{

}

void GraphicPiece::SetEmpty()
{
    NULLPTR(m_Shape);
}

#pragma once
#include "TicTacToe.h"

class GraphicPiece : public TicTacToe::Piece
{
public:

    GraphicPiece();
    ~GraphicPiece() override;

    void Clear() override;
    
    void SetPosition(sf::Vector2f position) { m_Position = position; };

private:

    sf::Vector2f m_Position;
    sf::Shape* m_Shape;

};

class GraphicBoard : public TicTacToe::Board
{
public:

    GraphicBoard() : Board(3, 3, DEFAULT_WINNABLE_PIECES) {};
    GraphicBoard(size_t width, size_t height, int winnablePieces, int piecePixelSize);
    ~GraphicBoard() override;

    void Init() override;
    void AddPlayerPieceInBoard(int cell, GraphicPiece* shape);
    void RemovePlayerPieceInBoard(int cell);

    int GetPieceSize() { return m_PiecePixelSize; };

private:

    int m_PiecePixelSize = DEFAULT_PIECE_SIZE;
    std::map<int, GraphicPiece*> m_AllPiecesOnBoard;

};


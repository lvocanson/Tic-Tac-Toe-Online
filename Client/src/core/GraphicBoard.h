#pragma once
#include "TicTacToe.h"

class GraphicPiece : public TicTacToe::Piece
{
public:

    GraphicPiece();
    ~GraphicPiece() override;

    void Clear() override;

    void SetShape(sf::Shape* shape) { m_Shape = shape; };
    void SetPosition(sf::Vector2f position) const { m_Shape->setPosition(position); };
    sf::Vector2f GetPosition() const { return m_Shape->getPosition(); };

private:

    sf::Shape* m_Shape;

};

class GraphicBoard : public TicTacToe::Board
{
public:

    GraphicBoard() : Board(3, 3, DEFAULT_WINNABLE_PIECES) {}
    GraphicBoard(size_t width, size_t height, int winnablePieces, int piecePixelSize);
    ~GraphicBoard() override;

    void Init() override;

    void AddPlayerPieceInBoard(int cell, const TicTacToe::Player* player, GraphicPiece* piece);
    void RemovePlayerPieceInBoard(int cell);

    GraphicPiece& GetGraphicPiece(int cell) { return *m_AllPiecesOnBoard[cell]; }
    int GetPieceSize() const { return m_PiecePixelSize; }

private:

    int m_PiecePixelSize = DEFAULT_PIECE_SIZE;
    std::map<int, GraphicPiece*> m_AllPiecesOnBoard;

};


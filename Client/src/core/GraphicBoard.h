#pragma once
#include "TicTacToe.h"

class GraphicPiece : public TicTacToe::Piece
{
public:

    GraphicPiece();
    ~GraphicPiece() override;

    void SetEmpty() override;

    void SetShape(sf::Shape* shape) { m_Shape = shape; };
    void SetPosition(sf::Vector2f position) const { m_Shape->setPosition(position); };
    sf::Vector2f GetPosition() const { return m_Shape->getPosition(); };

private:

    sf::Shape* m_Shape;

};

class GraphicBoard : public TicTacToe::Board
{
public:
    GraphicBoard() : GraphicBoard(DEFAULT_BOARD_ARGS, DEFAULT_PIECE_SIZE) {}
    GraphicBoard(size_t width, size_t height, unsigned int alignementGoal, float piecePixelSize);
    ~GraphicBoard() override;

    void Init();

    void AddPlayerPieceInBoard(unsigned int cell, const TicTacToe::Player* player, GraphicPiece* piece);
    void RemovePlayerPieceInBoard(unsigned int cell);

    GraphicPiece& GetGraphicPiece(unsigned int cell) { return *m_AllPiecesOnBoard[cell]; }
    float GetPieceSize() const { return m_PiecePixelSize; }

private:

    float m_PiecePixelSize = DEFAULT_PIECE_SIZE;
    std::map<unsigned int, GraphicPiece*> m_AllPiecesOnBoard;

};


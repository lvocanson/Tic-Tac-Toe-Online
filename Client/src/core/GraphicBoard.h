#pragma once
#include "TicTacToe.h"

class Player;

class GraphicPiece
{
public:

    GraphicPiece();
    ~GraphicPiece();

    void SetEmpty();

    void SetShape(sf::Shape* shape) { m_Shape = shape; };
    void SetPosition(sf::Vector2f position) const { m_Shape->setPosition(position); };
    sf::Vector2f GetPosition() const { return m_Shape->getPosition(); };

private:

    sf::Shape* m_Shape;

};

class GraphicBoard final : public TicTacToe::Board
{
public:

    GraphicBoard() : GraphicBoard(DEFAULT_BOARD_ARGS, DEFAULT_PIECE_SIZE) {}
    GraphicBoard(size_t width, size_t height, unsigned int alignementGoal, float piecePixelSize);
    ~GraphicBoard() override;

    void Init(Window* window);
    void DrawBoard();

    void InstanciateNewPlayerShape(const TicTacToe::PieceID pieceId, const unsigned int cell);
    void RemoveLastPlayerShape();

    GraphicPiece& GetGraphicPiece(unsigned int cell) { return *m_AllPiecesOnBoard[cell]; }
    float GetPieceSize() const { return m_PiecePixelSize; }

    void SetEmpty() override;
    void ClearBoardShapes();

private:

    float m_PiecePixelSize = DEFAULT_PIECE_SIZE;

    std::map<unsigned int, GraphicPiece*> m_AllPiecesOnBoard;
    std::vector<sf::Drawable*> m_PlayerShapes;

    Window* m_Window;
};


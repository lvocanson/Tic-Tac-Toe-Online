#include "src/pch.h"
#include "src/core/Player.h"

#include "GraphicBoard.h"

#include "PlayerPieceShape.h"

GraphicBoard::GraphicBoard(size_t width, size_t height, unsigned int winnablePieces, float piecePixelSize) : Board(width, height, winnablePieces), m_PiecePixelSize(piecePixelSize)
{
}

GraphicBoard::~GraphicBoard()
{
	SetEmpty();
	ClearBoardShapes();
}

void GraphicBoard::Init(unsigned int totalColumn, unsigned int totalRow, Window* window)
{
	Resize(totalColumn, totalRow);

	for (unsigned int i = 0; i < m_Size; i++)
	{
		m_AllPiecesOnBoard.insert(std::pair(i, new GraphicPiece()));
	}

	m_Window = window;
}

void GraphicBoard::DrawBoard()
{
	const float pieceSize = m_PiecePixelSize;
	const size_t width = GetWidth();
	const size_t height = GetHeight();
	const sf::Vector2f center = m_Window->GetCenter();

	// Draw the board - temp
	for (unsigned int i = 0; i < m_Size; ++i)
	{
		auto* square = new sf::RectangleShape(sf::Vector2f(pieceSize, pieceSize));
		square->setFillColor(sf::Color::Color(51, 56, 63));
		square->setOutlineColor(sf::Color::Color(0, 189, 156));
		square->setOutlineThickness(OUTLINE_THICKNESS);
		square->setPosition(center.x - (width * pieceSize * 0.5f) + (i % width) * pieceSize + OUTLINE_THICKNESS * (i % width),
			center.y - (height * pieceSize * 0.5f) + (i / height) * pieceSize + OUTLINE_THICKNESS * (i / height));

		m_Window->RegisterDrawable(square);
		GetGraphicPiece(i).SetShape(square);
		GetGraphicPiece(i).SetPosition(square->getPosition());
	}
}

void GraphicBoard::InstanciateNewPlayerShape(const TicTacToe::PieceID pieceId, const PlayerShapeType shapeType, unsigned int cell)
{
	auto pos = sf::Vector2f(GetGraphicPiece(cell).GetPosition());

	// Center the piece
	pos.x += GetPieceSize() * 0.5f;
	pos.y += GetPieceSize() * 0.5f;

	const auto playerPieceShape = new PlayerPieceShape(pieceId, shapeType, pos);
	m_PlayerShapes.push_back(playerPieceShape);
	m_Window->RegisterDrawable(playerPieceShape);
}

void GraphicBoard::RemoveLastPlayerShape()
{
	sf::Drawable* lastShape = m_PlayerShapes.back();

	m_Window->UnregisterDrawable(lastShape);
	RELEASE(lastShape);
	m_PlayerShapes.pop_back();
}

void GraphicBoard::SetEmpty()
{
	Board::SetEmpty();

	for (auto& playerShape : m_PlayerShapes)
	{
		m_Window->UnregisterDrawable(playerShape);
		RELEASE(playerShape);
	}

	m_PlayerShapes.clear();
}

void GraphicBoard::ClearBoardShapes()
{
	for (auto& piece : m_AllPiecesOnBoard)
	{
		RELEASE(piece.second)
	}

	m_AllPiecesOnBoard.clear();
}

GraphicPiece::GraphicPiece() : m_Shape(nullptr)
{
}

GraphicPiece::~GraphicPiece()
{
	SetEmpty();
}

void GraphicPiece::SetEmpty()
{
	NULLPTR(m_Shape);
}

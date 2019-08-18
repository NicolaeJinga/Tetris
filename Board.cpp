#include "Board.h"
#include<iostream>
Board::~Board()
{
	delete[] m_cells;
}

void Board::Init(sf::Vector2i matrixSize, sf::Vector2f graphicPosition, sf::Vector2f graphicSize)
{
	m_matrixSize = matrixSize;
	m_cells = new int[m_matrixSize.x*m_matrixSize.y];
	for(int i = 0 ; i < m_matrixSize.x*m_matrixSize.y ; ++ i)
	{
		m_cells[i] = -1;
	}
	m_graphicPosition = graphicPosition;
	m_graphicSize = graphicSize;
	m_cellSize.x = graphicSize.x / matrixSize.x;
	m_cellSize.y = graphicSize.y / matrixSize.y;
}

int& Board::At(sf::Vector2i position)
{
	return m_cells[m_matrixSize.x * position.y + position.x];
}

void Board::Imprint(Piece& piece)
{
	sf::Vector2i pos;
	for(int i = 0 ; i < piece.m_size.y ; ++ i)
	{
		for(int j = 0 ; j < piece.m_size.x ; ++ j)
		{
			pos.x = j + piece.m_position.x;
			pos.y = i + piece.m_position.y;

			if(pos.y >= 0 && pos.x >= 0 &&
				pos.y <  m_matrixSize.y && pos.x < m_matrixSize.x)
				if(At(pos) == -1 && piece.m_cells[i][j] == 1)
					At(pos) = piece.m_type;
		}
	}
}

void Board::Deprint(Piece& piece)
{
	sf::Vector2i pos;
	for(int i = 0 ; i < piece.m_size.y ; ++ i)
	{
		for(int j = 0 ; j < piece.m_size.x ; ++ j)
		{
			pos.x = j + piece.m_position.x;
			pos.y = i + piece.m_position.y;

			if(pos.y >= 0 && pos.x >= 0 && pos.y < m_matrixSize.y && pos.x < m_matrixSize.x)
				if(At(pos) == piece.m_type && piece.m_cells[i][j] == 1)
					At(pos) = -1;
		}
	}
}

Board::ECollisions::Enum Board::ComputeCollision(Piece& piece)
{
	sf::Vector2i pos;
	ECollisions::Enum returnCode = ECollisions::None;
	for(int i = 0 ; i < piece.m_size.y ; ++ i)
	{
		for(int j = 0 ; j < piece.m_size.x ; ++ j)
		{
			pos.x = j + piece.m_position.x;
			pos.y = i + piece.m_position.y;
			if(piece.m_cells[i][j] == 1)
			{
				if(!(pos.y >= 0 && pos.x >= 0 && pos.y < m_matrixSize.y && pos.x < m_matrixSize.x))
				{
					returnCode = (ECollisions::Enum)(returnCode|ECollisions::Border);
				}
				if((pos.y >= 0 && pos.x >= 0 && pos.y < m_matrixSize.y && pos.x < m_matrixSize.x))
				{
					if(At(pos) != -1 )
					{
						returnCode = (ECollisions::Enum)(returnCode|ECollisions::Piece);
					}
				}
			}		
		}
	}
	return returnCode;
}


void Board::Update(float dt)
{
	
}

void Board::DrawRectangle(sf::RenderTarget& renderTarget, sf::Vector2f P, sf::Vector2f size)
{
	sf::VertexArray border;
	sf::Color color = sf::Color::Black;
	border.append(sf::Vertex(P,color));
	border.append(sf::Vertex(P + sf::Vector2f(size.x,0.0f),color));
	border.append(sf::Vertex(P + size,color));
	border.append(sf::Vertex(P + sf::Vector2f(0.0f,size.y),color));
	border.append(sf::Vertex(P,color));
	border.setPrimitiveType(sf::PrimitiveType::LinesStrip);
	renderTarget.draw(border);
	sf::VertexArray bgTint;
	color = sf::Color(0,0,0,100);
	bgTint.append(sf::Vertex(P,color));
	bgTint.append(sf::Vertex(P + sf::Vector2f(size.x,0.0f),color));
	bgTint.append(sf::Vertex(P + size,color));
	bgTint.append(sf::Vertex(P + sf::Vector2f(0.0f,size.y),color));
	bgTint.setPrimitiveType(sf::PrimitiveType::Quads);
	renderTarget.draw(bgTint);
}

void Board::Draw(sf::RenderTarget& renderTarget)
{
	
	DrawRectangle(renderTarget,m_graphicPosition,m_graphicSize);
	DrawRectangle(renderTarget,sf::Vector2f(m_graphicPosition.x-m_cellSize.x*4.3f,m_graphicPosition.y),sf::Vector2f(m_cellSize.x*4.2f,m_cellSize.y*4.2f));
	DrawRectangle(renderTarget,sf::Vector2f(0.2f+m_graphicPosition.x+m_graphicSize.x,m_graphicPosition.y),sf::Vector2f(m_cellSize.x*4,m_cellSize.y*13));

	sf::Sprite sprite;
	for(int i = 0 ; i < m_matrixSize.y ; ++ i)
	{
		for(int j = 0 ; j < m_matrixSize.x ; ++ j)
		{
			if((Piece::EType)At(sf::Vector2i(j,i)) != -1)
			{
				sprite.setTexture(Piece::GetTexture((Piece::EType)At(sf::Vector2i(j,i))));
				sprite.setScale(m_cellSize.x/sprite.getTexture()->getSize().x,m_cellSize.y/sprite.getTexture()->getSize().y);
				sprite.setPosition(m_graphicPosition.x + j * m_cellSize.y,m_graphicPosition.y + i * m_cellSize.y);
				int value = 255;
				sprite.setColor(sf::Color(value,value,value,255));
				renderTarget.draw(sprite);
			}
		}
	}
}
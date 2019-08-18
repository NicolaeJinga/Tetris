#ifndef __BOARD_H__
#define __BOARD_H__

#include "SFML/Graphics.hpp"
#include "Piece.h"

class Board
{
public:
	int* m_cells;
	sf::Vector2i m_matrixSize;

	sf::Vector2f m_graphicSize;
	sf::Vector2f m_cellSize;
	sf::Vector2f m_graphicPosition;

	struct ECollisions
	{
		enum Enum
		{
			None = 0,
			Border = 1,
			Piece = 2
		};
	};

	~Board();

	void Init(sf::Vector2i matrixSize, sf::Vector2f graphicPosition, sf::Vector2f graphicSize);
	int& At(sf::Vector2i position);

	void Imprint(Piece& piece);
	void Deprint(Piece& piece);

	ECollisions::Enum ComputeCollision(Piece& piece);	

	void Update(float dt);
	void Draw(sf::RenderTarget& renderTarget);
private:
	void DrawRectangle(sf::RenderTarget& renderTarget, sf::Vector2f P, sf::Vector2f size);
};


#endif // __BOARD_H__
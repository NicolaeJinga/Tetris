#ifndef __PIECE_H__
#define __PIECE_H__

#include "SFML/Graphics.hpp"

class Piece
{
public:
	enum EType
	{
		LINE = 0,
		SQUARE,
		T,
		L,
		J,
		Z,
		S,
		COUNT
	};

	static int Storage[COUNT][4][4];

	EType m_type;
	sf::Vector2i m_position;
	sf::Vector2i m_size;
	int m_cells[4][4];

	sf::Vector2f m_cellGraphicalSize;
	sf::Vector2f m_boardGraphicPos;

	sf::Clock m_time;
	float m_fallTimeAccumulator;
	float m_timeSeated;
	sf::Vector2i m_lastSeatPosition;

	sf::Vector2f m_geometricCenter;

	bool m_isShadow;
	bool m_isStationary;
	
	static sf::Texture* m_textures[COUNT];
	
	static void LoadFromFile();
	void Init(sf::Vector2i position, EType type, sf::Vector2f cellSize, sf::Vector2f boardGraphicPos, bool isShadow = false, bool isStationary = false);
	void AssignData(EType type);
	static void LoadTextures();
	static sf::Texture& GetTexture(EType type);
	void Rotate(bool clockwise);
	void CopyTransform(const Piece& piece);
	void ResetFallTime();
	void AddTimeSeated(float dt);
	void ResetTimeSeated();
	float GetTimeSeated();
	void ComputeGeometricCenter();
	void Update(float dt);
	void Draw(sf::RenderTarget& renderTarget);
};

#endif //__PIECE_H__



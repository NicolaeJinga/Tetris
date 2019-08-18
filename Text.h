#ifndef __TEXT_H__
#define __TEXT_H__

#include "SFML/Graphics.hpp"

class Text
{
public:
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	float m_timeElapsed;
	float m_lifetime;

	void Init(sf::Vector2f position, sf::Vector2f velocity, float lifetime);
	void Update(float dt);
	void Draw(sf::RenderTarget& renderTarget);
};

#endif //__TEXT_H__
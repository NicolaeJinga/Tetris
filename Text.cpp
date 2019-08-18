#include "Text.h"

void Text::Init(sf::Vector2f position, sf::Vector2f velocity, float lifetime)
{
	
}

void Text::Update(float dt)
{
	m_position += m_velocity * dt;
}

void Text::Draw(sf::RenderTarget& renderTarget)
{

}
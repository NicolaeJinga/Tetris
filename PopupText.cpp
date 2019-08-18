#include "PopupText.h"

void PopupText::Init(const sf::Font& font, sf::String message, sf::Vector2f position, float lifetime, sf::Color color, float height, sf::Vector2f velocity /* = sf::Vector2f */)
{
	m_text.setFont(font);
	m_text.setString(message);
	m_text.setFillColor(color);
	m_text.setCharacterSize((int)(height*m_dpu.y));
	m_text.setOrigin(m_text.getLocalBounds().left + m_text.getLocalBounds().width/2.0f, m_text.getLocalBounds().top + m_text.getLocalBounds().height/2.0f);
	m_text.setScale(1.0f/m_dpu.x,1.0f/m_dpu.y);
	SetPosition(position);
	m_lifetime = lifetime;
	m_elapsedTime = 0.0f;
}

void PopupText::SetPosition(sf::Vector2f position)
{
	m_position = position;
	m_text.setPosition(m_position);
}

void PopupText::SetMessage(sf::String message)
{
	m_text.setString(message);
}

void PopupText::SetColor(sf::Color color)
{
	m_text.setFillColor(color);
}

bool PopupText::IsAlive()
{
	if(m_elapsedTime >= m_lifetime)
		return false;
	return true;
}

void PopupText::Update(float dt)
{
	m_elapsedTime += dt;
	m_position += m_velocity*dt;
	m_text.setPosition(m_position);
}

void PopupText::Draw(sf::RenderTarget& renderTarget)
{
	float alfa = 255*(1-sqrt(m_elapsedTime/m_lifetime));
	sf::Color c = m_text.getFillColor();
	c.a = (sf::Uint8)alfa;
	m_text.setFillColor(c);
	renderTarget.draw(m_text);
}

sf::Vector2f PopupText::m_dpu;
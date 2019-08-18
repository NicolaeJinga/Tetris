#ifndef __POPUP_TEXT_H__
#define __POPUP_TEXT_H__

#include<SFML/Graphics.hpp>

class PopupText
{
public:
	static sf::Vector2f m_dpu;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	float m_elapsedTime;
	float m_lifetime;
	sf::Text m_text;

	void Init(const sf::Font& font, sf::String message, sf::Vector2f position, float lifetime, sf::Color color, float height, sf::Vector2f velocity = sf::Vector2f(0.0f,0.0f));
	void SetPosition(sf::Vector2f position);
	void SetMessage(sf::String message);
	void SetColor(sf::Color color);
	bool IsAlive();
	void Update(float dt);
	void Draw(sf::RenderTarget& renderTarget);
};

#endif // __POPUP_TEXT_H__
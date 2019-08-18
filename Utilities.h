#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include "SFML/Graphics.hpp"

namespace Utilities
{
	float GetAngle(sf::Vector2f v);
	sf::Vector2f CreateUnitVectorFromAngle(float angle);
	float Dot(sf::Vector2f v, sf::Vector2f u);
	float Magnitude(const sf::Vector2f& v);
	float Distance(sf::Vector2f A, sf::Vector2f B);
	sf::Vector2f Normalized(const sf::Vector2f& v);
	bool CollisionCircleWithCircle(sf::Vector2f c1, float r1, sf::Vector2f c2, float r2);
	bool CollisionCircleWithLine(sf::Vector2f center, float radius, sf::Vector2f A, sf::Vector2f B);
	bool CollisionCircleWithSegment(sf::Vector2f C, float r, sf::Vector2f A, sf::Vector2f B);
	sf::Vector2f Reflect(sf::Vector2f n, sf::Vector2f v);

	sf::Vector2f PointA(sf::Vector2f position, sf::Vector2f normal, float length, float height);
	sf::Vector2f PointB(sf::Vector2f position, sf::Vector2f normal, float length, float height);
	sf::Vector2f PointC(sf::Vector2f position, sf::Vector2f normal, float length, float height);
	sf::Vector2f PointD(sf::Vector2f position, sf::Vector2f normal, float length, float height);

	void RotateSquareMatrixClockwise(int* matrix, int n);
	void RotateSquareMatrixCounterClockwise(int* matrix, int n);
}

#endif // __UTILITIES_H__
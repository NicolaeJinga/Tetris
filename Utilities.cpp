#include<cmath>

#include "Utilities.h"


namespace Utilities
{
	const float pi = 3.14159265f;

	float GetAngle(sf::Vector2f v)
	{
		return 180*atan2(v.y,v.x)/pi;
	}

	sf::Vector2f CreateUnitVectorFromAngle(float angle)
	{
		angle = angle * pi / 180.0f;
		return sf::Vector2f(cos(angle),sin(angle));
	}

	float Dot(sf::Vector2f u, sf::Vector2f v)
	{
		return u.x*v.x + u.y*v.y;
	}

	float Cross(sf::Vector2f u, sf::Vector2f v)
	{
		return u.x*v.y - u.y*v.x;
	}

	float Magnitude(const sf::Vector2f& v)
	{
		return sqrt(Dot(v,v));
	}

	float Distance(sf::Vector2f A, sf::Vector2f B)
	{
		return sqrt((A.x-B.x)*(A.x-B.x)+(A.y-B.y)*(A.y-B.y));
	}

	sf::Vector2f Normalized(const sf::Vector2f& v)
	{
		return v/Magnitude(v);
	}

	bool CollisionCircleWithCircle(sf::Vector2f c1, float r1, sf::Vector2f c2, float r2)
	{
		if(Distance(c1,c2) <= r2+r1)
			return true;
		return false;
	}

	bool CollisionCircleWithLine(sf::Vector2f center, float radius, sf::Vector2f A, sf::Vector2f B)
	{
		float distance = 0;
		sf::Vector2f v = B-A;
		float vMagn = Magnitude(B-A);
		sf::Vector2f v0 = v/vMagn;
		sf::Vector2f n0;
		n0.x = -v0.y;
		n0.y = v0.x;
		distance = Dot(center-A,n0);
		if(distance < radius)
			return true;
		return false;
	}

	bool CollisionCircleWithSegment(sf::Vector2f C, float r, sf::Vector2f A, sf::Vector2f B)
	{
		sf::Vector2f Acs = A-C;
		sf::Vector2f Bcs = B-C;
		sf::Vector2f v = Bcs - Acs;
		float alfa	= Dot(v,v);
		float beta	= Dot(Acs,v) * 2.0f;
		float gamma = Dot(Acs,Acs) - r*r;
		float delta = beta*beta-4*alfa*gamma;
		if(delta<0)
			return false;
		float t0	= (-beta-sqrt(delta)) / (2*alfa);
		float t1	= (-beta+sqrt(delta)) / (2*alfa);

		//		* * |---|
		if (t0 < 0 && t1 < 0)
			return false;
		//		|---| * *
		if (t0 > 1 && t1 > 1)
			return false;
		//		|-*-*-|
		if ( t0 > 0 && t0 < 1 && t1 < 1 && t1 > 0)
			return true;
		//		* |---| *
		if (t0 < 0 && t1 > 1)
			return true;
		//		* |-*--|
		if (t0 < 0 && t1 < 1 && t1 > 0)
			return true;
		//		|-*--| *
		if(t0 > 0 && t0 < 1 && t1 > 1)
			return true;

		return false;
	}

	sf::Vector2f Reflect(sf::Vector2f n, sf::Vector2f v)
	{
		sf::Vector2f y = Dot(v,n)*n;
		sf::Vector2f x = Dot(v,sf::Vector2f(n.y,-n.x))*sf::Vector2f(n.y,-n.x);
		return -x+y;
	}

	sf::Vector2f PointA(sf::Vector2f position, sf::Vector2f normal, float length, float height)
	{
		sf::Vector2f n0 = sf::Vector2f(-normal.y, normal.x);
		return position + (n0 * length/2.0f) + (normal * height/2.0f);
	}

	sf::Vector2f PointB(sf::Vector2f position, sf::Vector2f normal, float length, float height)
	{
		sf::Vector2f n0 = sf::Vector2f(-normal.y, normal.x);
		return position + (n0 * -length/2.0f) + (normal * height/2.0f);
	}

	sf::Vector2f PointC(sf::Vector2f position, sf::Vector2f normal, float length, float height)
	{
		sf::Vector2f n0 = sf::Vector2f(-normal.y, normal.x);
		return position + (n0 * -length/2.0f) + (normal * -height/2.0f);
	}

	sf::Vector2f PointD(sf::Vector2f position, sf::Vector2f normal, float length, float height)
	{
		sf::Vector2f n0 = sf::Vector2f(-normal.y, normal.x);
		return position + (n0 * length/2.0f) + ( normal * -height/2.0f);
	}

	void RotateSquareMatrixClockwise(int* matrix, int n)
	{
		int* a = new int[n*n];
		memcpy(a,matrix,n*n*sizeof(int));

		for(int i = 0 ; i < n ; ++ i)
			for(int j = 0 ; j < n ; ++ j)
				matrix[i*n+j] = a[(n-j-1)*n+i];
		delete[] a;
	}

	void RotateSquareMatrixCounterClockwise(int* matrix, int n)
	{
		int* a = new int [n*n];
		memcpy(a,matrix,n*n*sizeof(int));

		for(int i = 0 ; i < n ; ++ i)
			for(int j = 0 ; j < n ; ++ j)
				matrix[i*n+j] = a[j*n+(n-i-1)];
		delete[] a;
	}
}
#pragma once

#include "SFML/Graphics.hpp"

namespace sfe
{
	inline float Length(sf::Vector2f & Vec)
	{
		return sqrt(Vec.x*Vec.x + Vec.y*Vec.y);
	}

	inline bool Colides(sf::Vector2u Point, sf::Vector2u RectangleStart, sf::Vector2u RectangleEnd)
	{
		return RectangleStart.x <= Point.x && RectangleEnd.x >= Point.x && RectangleStart.y <= Point.y && RectangleEnd.y >= Point.y;
	}

}
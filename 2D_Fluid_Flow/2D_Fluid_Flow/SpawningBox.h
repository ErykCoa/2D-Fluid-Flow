#pragma once
#include "SFML/Graphics.hpp"
#include <random>


class SpawningBox 
{
public:
	sf::Rect<unsigned> Field;

	float Thickness;
	sf::Vector2f StartingVolacity;
	std::normal_distribution<float> PositionXDis;
	std::normal_distribution<float> PositionYDis;
	std::normal_distribution<float> VolacityXDis;
	std::normal_distribution<float> VolacityYDis;
	
	SpawningBox(sf::Rect<unsigned> Field, float Thickness, sf::Vector2f & StartingVolacity);
};


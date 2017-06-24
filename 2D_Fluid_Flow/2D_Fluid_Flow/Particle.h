#pragma once

#include "SFML/Graphics.hpp"
#include "Consts.h"
#include "sfe.h"
#include <array>
#include <cmath>
#include "stde.h"
#include "Settings.h"
#include "StructsAndEnums.h"
#include <random>

class Particle
{
public:
	void Collide(Particle & Particle);
	void Collide(std::vector<std::vector<bool>> & IsFieldOccupied);
	void CollideWithBorders();
	void Move();
	void Draw(sf::RenderWindow & Window);
	bool HasLeftMap();

	void Update();

	sf::Vector2f Position;
	sf::Vector2f Velocity;

	sf::Vector2f BoundryX;
	sf::Vector2f BoundryY;

	Particle(Settings & WinSettings, sf::Vector2f StartingPosition, sf::Vector2f StartingVelocity);
	Particle(
		Settings & WinSettings,
		sf::Vector2f StartingPosition, Boundaries<float> & PositionBoun, std::normal_distribution<float> & PositionDis,
		sf::Vector2f StartingVolacity, Boundaries<float> & VolacityBoun, std::normal_distribution<float> & VolacityDis
	);
	~Particle();

private:
	Settings & WinSettings;

	static std::default_random_engine Engine;

	void UpdateCircle(sf::Vector2f Scale, float Contrast);

	inline void SolidColision(sf::Vector2f & With);
	inline void SpringlikeColision(sf::Vector2f & With);

	sf::CircleShape Circle;

	sf::Vector2f NextPosition;
	sf::Vector2f NextVelocity;
};


#pragma once
#include "SFML/Graphics.hpp"
#include <string>

class FPSCounter
{
public:
	FPSCounter();

	std::string GetString();

	FPSCounter& operator++ ();
	FPSCounter& operator++ (int);

private:
	unsigned FPS;

	sf::Clock Clock;
	sf::Time TimeSinceLastUpdate;
	unsigned CurrentFrame;
};


#pragma once
#include "FPSCounter.h"

struct WindowInfo
{
	FPSCounter FPS;

	unsigned MapX;
	unsigned MapY;

	sf::Vector2f Scale;
};


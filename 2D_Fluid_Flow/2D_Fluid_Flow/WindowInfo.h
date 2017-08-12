#pragma once
#include "FPSCounter.h"
#include "StructsAndEnums.h"

struct WindowInfo
{
	FPSCounter FPS;

	unsigned MapX;
	unsigned MapY;

	sf::Vector2f Scale;

	BorderType LeftBorder;
	BorderType RightBorder;
	BorderType TopBorder;
	BorderType BottomBorder;

	WindowInfo() :
		Scale{1.f,1.f},
		MapX{1},
		MapY{1}
	{}
};


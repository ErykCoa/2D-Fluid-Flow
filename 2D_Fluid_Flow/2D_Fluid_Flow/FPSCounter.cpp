#include "FPSCounter.h"

FPSCounter & FPSCounter::operator++()
{
	++CurrentFrame;

	TimeSinceLastUpdate += Clock.restart();

	if (TimeSinceLastUpdate >= sf::milliseconds(1000))
	{
		FPS = CurrentFrame;
		CurrentFrame = 0;
		TimeSinceLastUpdate = sf::Time::Zero;
	}

	return *this;
}

FPSCounter & FPSCounter::operator++(int)
{
	++CurrentFrame;

	TimeSinceLastUpdate += Clock.restart();

	if (TimeSinceLastUpdate >= sf::milliseconds(1000))
	{
		FPS = CurrentFrame;
		CurrentFrame = 0;
		TimeSinceLastUpdate = sf::Time::Zero;
	}

	return *this;
}


FPSCounter::FPSCounter() :
	TimeSinceLastUpdate{ sf::Time::Zero },
	CurrentFrame{ 0 },
	FPS{ 0 }
{
}

std::string FPSCounter::GetString()
{
	return std::to_string(FPS);
}

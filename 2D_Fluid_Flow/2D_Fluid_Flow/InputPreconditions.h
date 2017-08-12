#pragma once
#include "Settings.h"
#include "SFML/Window/Event.hpp"

namespace InputPreconditions
{
	template<class T>
	bool IsOnScreen(Settings & WinSettings, sf::Vector2<T> & Coordinates);

	bool IsOnScreen(Settings & WinSettings, int RelativeX, int RelativeY);
	bool IsOnScreen(Settings & WinSettings, sf::Event::MouseButtonEvent & EventInfo);

};

template<class T>
bool InputPreconditions::IsOnScreen(Settings & WinSettings, sf::Vector2<T>& Coordinates)
{
	return (Coordinates.x < 0 || Coordinates.x > WinSettings.Info.MapX) ? false : (Coordinates.y < 0 || Coordinates.y > WinSettings.Info.MapY) ? false : true;
}
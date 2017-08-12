#pragma once
#include "InteractiveWindowInputEvents.h"
#include "SFML/Graphics.hpp"


	//const char MouseName[] = { "Mouse" };
	//const char KeyboardName[] = { "Keyboard" };


class EventInput {
public:
	InteractiveWindowInputEvents<sf::Event::EventType::MouseButtonPressed> MouseButtonPressed;
	InteractiveWindowInputEvents<sf::Event::EventType::KeyPressed> Keyboard;
};


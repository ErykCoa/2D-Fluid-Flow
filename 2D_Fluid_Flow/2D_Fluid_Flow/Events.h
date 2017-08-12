#pragma once
#include "SFML/Graphics.hpp"
#include "EventInput.h"
#include "Subject.h"

class Events
{
public:
	void PoleAllWindowEvents(sf::Window & From);

	EventInput Input;

	Subject<sf::Event> EventSubject;

	Events();
	~Events();
};

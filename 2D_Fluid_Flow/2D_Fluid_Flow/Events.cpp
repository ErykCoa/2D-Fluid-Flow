#include "Events.h"


void Events::PoleAllWindowEvents(sf::Window & From)
{
	sf::Event Event;
	while (From.pollEvent(Event))
	{
		EventSubject.SetState(Event);			
	}
}

Events::Events()
{
	EventSubject.Attach(Input.MouseButtonPressed);
	EventSubject.Attach(Input.Keyboard);
}


Events::~Events()
{
}

#include "MousePosition.h"



MousePosition::MousePosition(sf::RenderWindow & Window):
	Window(Window),
	CurrentPosition{}
{
}


void MousePosition::Update(sf::Event & State)
{
	if (State.type == sf::Event::MouseMoved)
	{
		CurrentPosition = sf::Vector2i(Window.mapPixelToCoords({ State.mouseMove.x, State.mouseMove.y }));
	}
}

sf::Vector2i MousePosition::GetPosition()
{
	return CurrentPosition;
}

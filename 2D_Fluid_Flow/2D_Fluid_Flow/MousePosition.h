#pragma once
#include "IObserver.h"
#include "SFML/Graphics.hpp"

class MousePosition : public IObserver<sf::Event>
{
public:	
	virtual void Update(sf::Event & State) override;

	sf::Vector2i GetPosition();

	MousePosition(sf::RenderWindow & Window);

private:
	sf::Vector2i CurrentPosition;
	sf::RenderWindow & Window;
};


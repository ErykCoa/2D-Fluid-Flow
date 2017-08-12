#include "SpawningBoxRep.h"


SpawningBoxRep::SpawningBoxRep(SpawningBox Box, WindowInfo & Info) :
	IObject(Info),
	SpawningBox(Box)
{
	Shape.setFillColor(sf::Color::Blue);
	AbsolutePosition = sf::Vector2f(Field.left, Field.top);
	Shape.setSize(sf::Vector2f(Field.width , Field.height ));
}

bool SpawningBoxRep::IsStatic()
{
	return true;
}

void SpawningBoxRep::Move()
{
	throw std::logic_error("Object is static!");
}

void SpawningBoxRep::Draw(sf::RenderTarget & Window)
{
	Shape.setPosition(sf::Vector2f(AbsolutePosition.x * Info.Scale.x, AbsolutePosition.y * Info.Scale.y));
	Shape.setScale(Info.Scale);

	switch (CurrentVisability)
	{
	case VisabilityType::Visible: Shape.setFillColor(sf::Color(0,0,255,150)); Window.draw(Shape); break;
	case VisabilityType::Selected: Shape.setFillColor(sf::Color(0,0,255,220)); Window.draw(Shape); break;
	default: break;
	}
}

bool SpawningBoxRep::IsAt(sf::Vector2u Coordinates)
{
	Shape.setPosition(sf::Vector2f(AbsolutePosition.x * Info.Scale.x, AbsolutePosition.y * Info.Scale.y));
	Shape.setScale(Info.Scale);

	return Shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(Coordinates));
}

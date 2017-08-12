#include "StaticObject.h"


StaticObject::StaticObject(WindowInfo & Info, const std::string & Path) :
	IObject(Info)
{
	LoadSpritePosition(Path);
	LoadTexture(Path);
	LoadHitBox(Path);
	UpdateHitBox();
}

StaticObject::~StaticObject()
{
}

bool StaticObject::IsStatic()
{
	return true;
}

void StaticObject::Move()
{
	throw std::logic_error("Object is static!");
}

void StaticObject::Draw(sf::RenderTarget & Window)
{
	Sprite.setPosition(sf::Vector2f(AbsolutePosition.x * Info.Scale.x, AbsolutePosition.y * Info.Scale.y));
	Sprite.setScale(Info.Scale);

	switch (CurrentVisability)
	{
	case VisabilityType::Default:
	case VisabilityType::Visible:
	case VisabilityType::Selected: Window.draw(Sprite); break;
	default: break;
	}
}



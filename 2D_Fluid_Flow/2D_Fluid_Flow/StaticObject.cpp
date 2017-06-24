#include "StaticObject.h"


StaticObject::StaticObject(Settings & WinSettings, const std::string & Path) :
	IObject(WinSettings)
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

void StaticObject::Draw(sf::RenderWindow & Window)
{
	Sprite.setScale(WinSettings.Info.Scale);
//	Window.draw(Sprite);
}

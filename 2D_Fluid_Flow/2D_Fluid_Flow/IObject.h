#pragma once
#include <vector>
#include "SFML/Graphics.hpp"
#include <memory>
#include <fstream>
#include "StructsAndEnums.h"
#include "stde.h"
#include "Consts.h"
#include <mutex>
#include "Settings.h"

class IObject
{
public:
	std::vector<OccupiedField> OccupiedFields;

	virtual bool IsStatic() = 0;
	virtual void Move() = 0;
	virtual void Draw(sf::RenderWindow & Window) = 0;
	virtual ~IObject() {};

	IObject(Settings & WinSettings) : Texture(new sf::Texture), WinSettings( WinSettings ) {}

protected:
	Settings & WinSettings;

	sf::Sprite Sprite;
	std::unique_ptr<sf::Texture> Texture;
	sf::Image Image;
	sf::Image HitBox;

	virtual void UpdateHitBox();
	virtual void LoadHitBox(const std::string & Path);
	virtual void LoadTexture(const std::string & Path);
	virtual void LoadSpritePosition(const std::string & Path);
};

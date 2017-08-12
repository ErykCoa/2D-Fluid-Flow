#pragma once
#include "SFML/Graphics.hpp"
#include "IObject.h"
#include "WindowInfo.h"
#include <functional>
#include "SpawningBox.h"

class SpawningBoxRep : public IObject, public SpawningBox
{
public:
	SpawningBoxRep(SpawningBox Box, WindowInfo & Info);

	virtual bool IsStatic() override;
	virtual void Move() override;
	virtual void Draw(sf::RenderTarget & Window) override;
	virtual bool IsAt(sf::Vector2u Coordinates) override;
private:
	sf::RectangleShape Shape;
};


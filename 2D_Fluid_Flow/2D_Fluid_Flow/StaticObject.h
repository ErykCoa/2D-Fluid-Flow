#pragma once

#include "IObject.h"
#include <memory>
#include <string>
#include <iostream>
#include <fstream>

class StaticObject : public IObject
{
public:
	StaticObject(Settings & WinSettings, const std::string & Path);
	~StaticObject();

	virtual bool IsStatic() override;
	virtual void Move() override;
	virtual void Draw(sf::RenderWindow & Window) override;
};


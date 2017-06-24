#pragma once

#include "SFML/Graphics.hpp"
#include <array>
#include "Consts.h"
#include <string>
#include "IObject.h"
#include <exception>
#include "ManagedResource.h"
#include "StructsAndEnums.h"
#include <iostream>
#include <fstream>
#include "StaticObject.h"
#include "FluidVelocityMap.h"
#include "Settings.h"
#include "TextCommands.h"

class PipeMap
{
public:
	void SingleTick();
	bool WindowHasBeenClosed();

	PipeMap(const std::string & Path);
	~PipeMap();
private:
	sf::RenderWindow Window;

	void Draw();
	void MovementTick();

	void LoadBackground(const std::string & File);
	void LoadObject(const std::string & Path);
	void LoadAllObjects(const std::string & Path);
	void SetFlowInitialSpeed(sf::Vector2f Speed);

	Settings WinSettings;
	FluidVelocityMap Map;
	TextCommands CommandsProcessing;

	float UnitsSinceLastRender;

	sf::Sprite Background;

	std::vector<std::unique_ptr<IObject>> Objects;

	ResourceHolder<sf::Texture, ID> Textures;

	void UpdateMap();
	void ObjectsMovement();
};


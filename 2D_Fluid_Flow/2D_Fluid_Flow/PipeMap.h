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
#include <memory.h>
#include <future>
#include <atomic>
#include "TimeCounter.h"
#include "WindowViewChanger.h"
#include "IObserver.h"


class PipeMap : public IObserver<sf::Event>
{
public:
	static std::unique_ptr<PipeMap> GetPipeMapFromUserInput();

	PipeMapState GetMapState();

	void StartSimulation();

	void SingleTick();

	virtual void Update(sf::Event & State) override;
	

	PipeMap(const std::string & Path);
	~PipeMap();
private:
	sf::RenderWindow Window;

	
	void Draw();
	void MovementTick();
	bool WindowHasBeenClosed();

	void LoadBackground(const std::string & File);
	void LoadObject(const std::string & Path);
	void LoadAllObjects();
	void SetFlowInitialSpeed(sf::Vector2f Speed);
	void ExecuteInitScript();

	Settings WinSettings;
	FluidVelocityMap Map;
	TextCommands CommandsProcessing;

	float TimeUnitsSinceLastRender;

	sf::Sprite Background;

	std::mutex SwapMutex;
	std::unique_ptr<sf::RenderTexture> LastFrame;
	std::unique_ptr<sf::RenderTexture> FrameBuffer;

	ResourceHolder<sf::Texture, ID> Textures;

	std::thread Render;

	TimeCounter TimePerTick;

	void UpdateMap();
	void ObjectsMovement();

	friend void th(PipeMap*);


};


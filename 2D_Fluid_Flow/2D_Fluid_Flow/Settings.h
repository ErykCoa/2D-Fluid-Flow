#pragma once

#include <string>
#include "stde.h"
#include <atomic>
#include "WindowInfo.h"
#include "Overlay.h"
#include "ConsoleIO.h"
#include <mutex>
#include "StructsAndEnums.h"
#include <experimental\filesystem>
#include "SFML/Window/Event.hpp"
#include "Events.h"
#include "ISpawningBoxes.h"
#include "MousePosition.h"
#include "SelectedWindowArea.h"
#include "ObjectsHandler.h"
#include "WindowViewChanger.h"
#include "IFluidVelocityMap.h"
#include <condition_variable>


class Settings
{
public:
	std::string & GetMapPath();
	std::atomic<PipeMapState> MapState;
	bool IsReadyForStart;

	bool IsMapPathSet();
	bool TrySetMapPath(const std::string & With);

	void UpdateTickDuration();
	void UpdateGreatestVolicity();
	void TrySetNextGreatestVolicity(float With);

	WindowInfo Info;
	MousePosition MousePosition;
	WindowViewChanger ViewChanger;
	Overlay Overlay;
	ConsoleIO Console;

	ISpawningBoxes * SpawningBoxes;
	IFluidVelocityMap * FluidMap;

	Events WindowEvents;

	

	SelectedWindowArea SelectedWindowArea;

	std::atomic<long long> ParticlesCounter;

	std::unique_ptr<IObjectsHandler> Objects;

	RenderType RenderType;



	float BaseSpringinessForce;
	float ParticleRadius;
	float TimeUnitsPerFrame;
	float Contrast;

	float CurrentTickDuration;
	float CurrentGreatestVolicity;
	std::atomic<float> NextGreatestVolicity;

	Settings(const std::string & MapPath, sf::RenderWindow & Window);
	Settings(sf::RenderWindow & Window);
private:
	std::string MapPath;

	void LoadConfiguration();
	bool IsPathCorrect(const std::string & MapPath);
};


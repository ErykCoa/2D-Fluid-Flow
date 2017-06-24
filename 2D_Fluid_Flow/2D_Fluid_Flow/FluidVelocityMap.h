#pragma once
#include "SFML/Graphics.hpp"
#include <array>
#include "Consts.h"
#include "StructsAndEnums.h"
#include "stde.h"
#include <forward_list>
#include <set>
#include "Particle.h"
#include <Windows.h>
#include <mutex>
#include "Settings.h"
#include "CommonFunctions.h"
#include <random>
#include "ParticleSpawner.h"

class FluidVelocityMap
{
public:
	void MovementTick();
	void Draw(sf::RenderWindow & Window);

	std::vector<std::vector<sf::Vector2f>> Map;

	std::vector<std::vector<bool>> IsFieldOccupied;
	std::vector<std::vector<bool>> IsFieldOccupiedStaticly;

	void FillUnoccupiedFields();
	
	void SetFlowInitialSpeed(sf::Vector2f Speed) { FlowInitialSpeed = Speed; }
	FluidVelocityMap(Settings & WinSettings);

private:
	Settings & WinSettings;

	ParticleSpawner Spawner;

	std::vector<std::forward_list<std::pair<unsigned, unsigned>>> NotOccupiedPaths;

	std::vector<std::vector<std::vector<Particle>>> Particles;
	std::vector<std::vector<std::vector<Particle>>> NextParticles;

	std::vector<std::mutex> MutArray;

	sf::Vector2f FlowInitialSpeed;

	sf::Sprite SpeedValueSprite;

	sf::Image SpeedValueImage;

	void MapBasicParticles();

	void RegularRender(sf::RenderWindow & Window);
	void CirclesRender(sf::RenderWindow & Window);

	void UpdatePaths();

	void UpdateSpeedValueImage();

	unsigned NotOccupiedFields(const unsigned Column);


};


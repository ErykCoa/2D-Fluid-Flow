#pragma once
#include "SFML/Graphics.hpp"
#include "Particle.h"
#include <utility>
#include <vector>
#include "StructsAndEnums.h"
#include "sfe.h"
#include "Settings.h"
#include <random>

class ParticleSpawner
{
public:
	Particle SpawnNew();
	void AddNewSpawningBox(SpawningBox & SpawningBox);

	void FindAndEraseSpawningBox(sf::Vector2u & Position);

	ParticleSpawner(Settings & WinSettings);

private:
	static std::default_random_engine Engine;
	static std::uniform_int_distribution<unsigned> Dis;

	Settings & WinSettings;
	std::vector<SpawningBox> SpawningBoxes;
};


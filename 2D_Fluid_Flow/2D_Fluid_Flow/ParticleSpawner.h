#pragma once
#include "SFML/Graphics.hpp"
#include "Particle.h"
#include <utility>
#include <vector>
#include "StructsAndEnums.h"
#include "sfe.h"
#include "Settings.h"
#include <random>
#include "ISpawningBoxes.h"
#include <functional>
#include "InputPreconditions.h"
#include "SpawningBoxRep.h"

class ParticleSpawner : public ISpawningBoxes, public IObserver<IObject*>
{
public:
	std::vector<Particle> SpawnParticlesTick();

	void SetCondition(std::function<bool(sf::Vector2f & Coordinetes)> AreCoordinatesCorrect);

	Particle SpawnNew();
	Particle SpawnNew(
		sf::Vector2f StartingPosition, Boundaries<float> & PositionBoun, std::normal_distribution<float> & PositionDis,
		sf::Vector2f StartingVolacity, Boundaries<float> & VolacityBoun, std::normal_distribution<float> & VolacityDis);
	
	void AddNewSpawningBox(SpawningBox & SpawningBox) override;
	void FindAndEraseSpawningBox(sf::Vector2u & Position) override;
	virtual void ResetClock() override;

	ParticleSpawner(Settings & WinSettings);

	virtual void Update(IObject *& State) override;

private:
	std::function<bool(sf::Vector2f & Coordinetes)> AreCoordinatesCorrect;
	sf::Clock Clock;
	sf::Time TimeSinceLastTick;

	static std::default_random_engine Engine;
	static std::uniform_int_distribution<unsigned> Dis;

	Settings & WinSettings;
	std::vector<SpawningBoxRep*> SpawningBoxes;
};


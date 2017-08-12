#pragma once

#include <vector>
#include "StructsAndEnums.h"
#include <atomic>
#include <functional>
#include "SpawningBox.h"

class ISpawningBoxes {
public:
	virtual void AddParticlesToSpawn(unsigned Count) { ParticlesToSpawn += Count; }
	virtual void SetMaxParticlesPerSecond(unsigned Count) { MaxParticlesPerTick.store(Count); }

	virtual void AddNewSpawningBox(SpawningBox & SpawningBox) = 0;
	virtual void FindAndEraseSpawningBox(sf::Vector2u & Position) = 0;
	virtual void ResetClock() = 0;

protected:
	std::atomic<unsigned> ParticlesToSpawn;
	std::atomic<unsigned> MaxParticlesPerTick;
};
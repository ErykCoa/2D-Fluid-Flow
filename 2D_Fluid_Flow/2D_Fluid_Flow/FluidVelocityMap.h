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
#include <random>
#include "ParticleSpawner.h"
#include <atomic>
#include "TimeCounter.h"
#include "IObserver.h"
#include "IObject.h"
#include "IFluidVelocityMap.h"

class FluidVelocityMap : public IObserver<IObject*>, public IFluidVelocityMap
{
public:
	void MovementTick();
	void Draw(sf::RenderTarget & Window);

	std::vector<std::vector<sf::Vector2f>> Map;

	std::vector<std::vector<bool>> IsFieldOccupied;
	std::vector<std::vector<bool>> IsFieldOccupiedStaticly;

	void FillUnoccupiedFields() override;
	
	void SetFlowInitialSpeed(sf::Vector2f Speed) { FlowInitialSpeed = Speed; }
	FluidVelocityMap(Settings & WinSettings);

	std::atomic<bool> HasBeenRendered;

	virtual void Update(IObject *& State) override;

private:
	Settings & WinSettings;

	ParticleSpawner Spawner;

	std::vector<std::forward_list<std::pair<unsigned, unsigned>>> NotOccupiedPaths;

	std::vector<std::vector<std::vector<Particle>>> Particles;
	std::vector<std::vector<std::vector<Particle>>> NextParticles;

	std::vector<std::mutex> MutArray;

	sf::Vector2f FlowInitialSpeed;

	sf::Sprite SpeedValueSprite;
	sf::Texture SpeedValueTexture;
	sf::Image SpeedValueImage;

	TimeCounter TickDuration;

	void MapBasicParticles();

	void RegularRender(sf::RenderTarget & Window);
	void CirclesRender(sf::RenderTarget & Window);

	void UpdatePaths();

	void UpdateSpeedValueImage();

	unsigned NotOccupiedFields(const unsigned Column);
};


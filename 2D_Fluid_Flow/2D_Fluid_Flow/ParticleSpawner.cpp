#include "ParticleSpawner.h"

std::default_random_engine ParticleSpawner::Engine{};
std::uniform_int_distribution<unsigned> ParticleSpawner::Dis{};


Particle ParticleSpawner::SpawnNew()
{
	unsigned CombinedChance{};

	for (auto& Item : SpawningBoxes)
	{
		CombinedChance += Item.Field.height * Item.Field.width * Item.Thickness;
	}

	int RandomNum = Dis(Engine) % CombinedChance;

	for (auto& Item : SpawningBoxes)
	{
		RandomNum -= Item.Field.height * Item.Field.width * Item.Thickness;

		if (RandomNum <= 0)
		{
			++WinSettings.ParticlesCounter;

			return Particle(
				WinSettings,
				{ stde::Trim(Item.PositionXDis(Engine), Item.Field.left, Item.Field.left + Item.Field.width),
				stde::Trim(Item.PositionYDis(Engine), Item.Field.top, Item.Field.top + Item.Field.height) },
				{ Item.VolacityXDis(Engine), Item.VolacityYDis(Engine) }
			);
		}
	}
}

Particle ParticleSpawner::SpawnNew(sf::Vector2f StartingPosition, Boundaries<float>& PositionBoun, std::normal_distribution<float>& PositionDis, sf::Vector2f StartingVolacity, Boundaries<float>& VolacityBoun, std::normal_distribution<float>& VolacityDis)
{
	auto Position = stde::Trim(sf::Vector2f{ StartingPosition.x + PositionDis(Engine), StartingPosition.y + PositionDis(Engine) }, PositionBoun);
	auto Velocity = stde::Trim(sf::Vector2f{ StartingVolacity.x + VolacityDis(Engine), StartingVolacity.y + VolacityDis(Engine) }, VolacityBoun);

	++WinSettings.ParticlesCounter;

	return Particle(
		WinSettings,
		Position,
		Velocity
	);
}


void ParticleSpawner::AddNewSpawningBox(SpawningBox & SpawningBox)
{
	SpawningBoxes.push_back(SpawningBox);
}

void ParticleSpawner::FindAndEraseSpawningBox(sf::Vector2u & Position)
{
	for (auto Iter = SpawningBoxes.begin(); Iter != SpawningBoxes.end(); ++Iter)
		if (Iter->Field.contains(Position))
		{
			SpawningBoxes.erase(Iter);
			return;
		}

	throw std::runtime_error("No box was found.");
}

ParticleSpawner::ParticleSpawner(Settings & WinSettings):
	WinSettings(WinSettings)
{
}


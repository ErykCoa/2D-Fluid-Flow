#include "ParticleSpawner.h"

std::default_random_engine ParticleSpawner::Engine{};
std::uniform_int_distribution<unsigned> ParticleSpawner::Dis{};


std::vector<Particle> ParticleSpawner::SpawnParticlesTick()
{
	auto ElapsedTime = Clock.restart();
	TimeSinceLastTick += sf::seconds(WinSettings.CurrentTickDuration);

	std::vector<Particle> Res{};

	try {
		if (TimeSinceLastTick.asMilliseconds() < 0)
			TimeSinceLastTick = sf::Time::Zero;

		auto PerticlesPerTick = MaxParticlesPerTick.load() * TimeSinceLastTick.asMilliseconds() / 1000.f;

		if (PerticlesPerTick - 1.f > 0)
		{

			for (unsigned Counter = 0; Counter < PerticlesPerTick - 1.f; ++Counter)
			{
				TimeSinceLastTick -= sf::seconds(WinSettings.CurrentTickDuration);
				Res.push_back(SpawnNew());
			}


		}

	}
	catch (std::runtime_error &)
	{

	}

	return Res;
}

void ParticleSpawner::SetCondition(std::function<bool(sf::Vector2f&Coordinetes)> AreCoordinatesCorrect)
{
	this->AreCoordinatesCorrect = AreCoordinatesCorrect;
}

Particle ParticleSpawner::SpawnNew()
{
	if (SpawningBoxes.empty())
		throw std::runtime_error("No Spawner!");

	while (true)
	{
		unsigned CombinedChance{};

		for (auto& Item : SpawningBoxes)
		{
			CombinedChance += Item->Field.height * Item->Field.width * Item->Thickness;
		}

		if (CombinedChance == 0)
			throw std::runtime_error("Chance == 0!");

		int RandomNum = Dis(Engine) % CombinedChance;

		for (auto& Item : SpawningBoxes)
		{
			RandomNum -= Item->Field.height * Item->Field.width * Item->Thickness;

			if (RandomNum <= 0)
			{
				Particle Res(
					WinSettings,
					{ stde::Trim(Item->PositionXDis(Engine), Item->Field.left, Item->Field.left + Item->Field.width -1),
					stde::Trim(Item->PositionYDis(Engine), Item->Field.top, Item->Field.top + Item->Field.height -1 ) },
					{ Item->VolacityXDis(Engine), Item->VolacityYDis(Engine) }
				);

				if (InputPreconditions::IsOnScreen(WinSettings, Res.Position) && AreCoordinatesCorrect(Res.Position))
				{
					WinSettings.ParticlesCounter += 1;
					return std::move(Res);
				}
			}
		}
	}
}

Particle ParticleSpawner::SpawnNew(sf::Vector2f StartingPosition, Boundaries<float>& PositionBoun, std::normal_distribution<float>& PositionDis, sf::Vector2f StartingVolacity, Boundaries<float>& VolacityBoun, std::normal_distribution<float>& VolacityDis)
{
	auto Position = stde::Trim(sf::Vector2f{ StartingPosition.x + PositionDis(Engine), StartingPosition.y + PositionDis(Engine) }, PositionBoun);
	auto Velocity = stde::Trim(sf::Vector2f{ StartingVolacity.x + VolacityDis(Engine), StartingVolacity.y + VolacityDis(Engine) }, VolacityBoun);

	Particle Res(
		WinSettings,
		Position,
		Velocity
	);


	if (InputPreconditions::IsOnScreen(WinSettings, Res.Position) && AreCoordinatesCorrect(Res.Position))
	{
		WinSettings.ParticlesCounter += 1;
		return std::move(Res);
	}

	throw std::runtime_error("Particle could not be spawned!");
}



void ParticleSpawner::AddNewSpawningBox(SpawningBox & Box)
{
	auto tmp = new SpawningBoxRep(Box, WinSettings.Info);
	WinSettings.Objects->AddNew(std::unique_ptr<IObject>(tmp));
	SpawningBoxes.push_back(tmp);
	
}

void ParticleSpawner::FindAndEraseSpawningBox(sf::Vector2u & Position)
{
	for (auto Iter = SpawningBoxes.rbegin(); Iter != SpawningBoxes.rend(); ++Iter)
		if ((*Iter)->Field.contains(Position))
		{
			SpawningBoxes.erase(Iter.base());
			return;
		}

	throw std::runtime_error("No box was found.");
}

void ParticleSpawner::ResetClock()
{
	Clock.restart();
	TimeSinceLastTick = sf::Time::Zero;
}

ParticleSpawner::ParticleSpawner(Settings & WinSettings):
	WinSettings(WinSettings)
{
	WinSettings.SpawningBoxes = this;
	WinSettings.Objects->Attach(*this);
}

void ParticleSpawner::Update(IObject *& State)
{
	for (auto Iter = SpawningBoxes.begin(); Iter != SpawningBoxes.end(); ++Iter)
	{
		if (State == *Iter)
		{
			SpawningBoxes.erase(Iter);
			return;
		}
	}
}


#include "FluidVelocityMap.h"


void FluidVelocityMap::FillUnoccupiedFields()
{
	std::normal_distribution<float> PositionDis{0.f, 0.2f};
	std::normal_distribution<float> VolacityDis{0.f, 0.05f};

	for (int X = 0; X < WinSettings.Info.MapX; X += WinSettings.ParticleRadius * 2.f)
	{
		for (int Y = 0; Y < WinSettings.Info.MapY; Y += WinSettings.ParticleRadius * 2.f)
		{
			if (!IsFieldOccupied[X][Y])
			{
				auto P = 
				Spawner.SpawnNew(
				{ X + 0.5f, Y + 0.5f }, Boundaries<float>{ (float)X + 0.01f, X + 0.99f, (float)Y + 0.01f, Y + 0.99f }, PositionDis,
				{ 0.f, 0.f }, Boundaries<float>{ -0.01f, 0.01f, -0.01f, 0.01f }, VolacityDis
				);

				Particles[P.Position.x][P.Position.y].push_back(P);
			}
		}
	}
}

void FluidVelocityMap::MovementTick()
{
	MapBasicParticles();
}

void FluidVelocityMap::Draw(sf::RenderTarget & Window)
{
	switch (WinSettings.RenderType)
	{
	case RenderType::Circles:
		CirclesRender(Window);
		break;
	case RenderType::Regular:
		RegularRender(Window);
		break;
	default:
		break;
	}

	HasBeenRendered.store(true);

}


FluidVelocityMap::FluidVelocityMap(Settings & WinSettings) :
	FlowInitialSpeed{ 10000.f, 0.f },
	WinSettings{ WinSettings },
	Particles(WinSettings.Info.MapX, std::vector<std::vector<Particle>>(WinSettings.Info.MapY)),
	NextParticles(WinSettings.Info.MapX, std::vector<std::vector<Particle>>(WinSettings.Info.MapY)),
	NotOccupiedPaths(WinSettings.Info.MapX),
	IsFieldOccupied( WinSettings.Info.MapX, std::vector<bool>(WinSettings.Info.MapY) ),
	IsFieldOccupiedStaticly( WinSettings.Info.MapX, std::vector<bool>(WinSettings.Info.MapY) ),
	MutArray(WinSettings.Info.MapX),
	Map(WinSettings.Info.MapX, std::vector<sf::Vector2f>(WinSettings.Info.MapY)),
	Spawner(WinSettings)
{
	for (int X = 0; X < WinSettings.Info.MapX; ++X)
		for (int Y = 0; Y < WinSettings.Info.MapY; ++Y)
		{
			Particles[X][Y].reserve(5);
			NextParticles[X][Y].reserve(5);
		}
	

	Spawner.SetCondition([&](sf::Vector2f & Position) {return !IsFieldOccupied[Position.x][Position.y]; });
	Spawner.SetMaxParticlesPerSecond(0);

	//WinSettings.Overlay.AddItem("TickDuration", [&]() {return TickDuration.GetString(); });
	WinSettings.Objects->Attach(*this);

	UpdatePaths();

	WinSettings.FluidMap = this;
}


void FluidVelocityMap::MapBasicParticles()
{
	auto Lock = TickDuration.MeasureTime();
//	static unsigned long long Counter = 0;
	

	for (auto & P : Spawner.SpawnParticlesTick())
		Particles[P.Position.x][P.Position.y].push_back(P);		
	

	stde::For_each<NumberOfThreadsRunning>(Particles.begin(), Particles.end(), [&](decltype(Particles.begin()) Iter)
	{
		auto x = std::distance(Particles.begin(), Iter);

		for (int Y = 0; Y < WinSettings.Info.MapY; ++Y)
		{
			for (auto& Item : Iter->data()[Y])
			{
				Item.Move();

				for (int IndexX = stde::Trim(Item.BoundryX.x, 0, WinSettings.Info.MapX); IndexX < Item.BoundryX.y && IndexX < WinSettings.Info.MapX; ++IndexX)
					for (int IndexY = stde::Trim(Item.BoundryY.x, 0, WinSettings.Info.MapY); IndexY < Item.BoundryY.y && IndexY < WinSettings.Info.MapY; ++IndexY)
						for (auto& With : Particles[IndexX][IndexY])
						{
							if (&Item == &With) continue;

							Item.Collide(With);
						}

				Item.Collide(IsFieldOccupied);

				Item.CollideWithBorders();
			}
		}

	});

	stde::For_each<NumberOfThreadsRunning>(Particles.begin(), Particles.end(), [&](decltype(Particles.begin()) Iter)
	{
		auto x = std::distance(Particles.begin(), Iter);
		for (int Y = 0; Y < WinSettings.Info.MapY; ++Y)
		{
			for (auto& Item : Iter->data()[Y])
			{
				Item.Update();

				if (Item.HasLeftMap())
				{
					--WinSettings.ParticlesCounter;
					continue;
				}


				std::lock_guard<std::mutex> _lock(MutArray[stde::Trim(Item.Position.x, 0, WinSettings.Info.MapX - 1)]);
				NextParticles[stde::Trim(Item.Position.x, 0, WinSettings.Info.MapX - 1)][stde::Trim(Item.Position.y, 0, WinSettings.Info.MapY - 1)].push_back(Item);
			}
			Particles[x][Y].clear();
		}
	});


	Particles.swap(NextParticles);


}

void FluidVelocityMap::RegularRender(sf::RenderTarget & Window)
{
	SpeedValueImage.create(WinSettings.Info.MapX, WinSettings.Info.MapY, sf::Color::Black);

	stde::For_each<NumberOfThreadsRunning>(Particles.begin(), Particles.end(), [&](decltype(Particles.begin()) Iter)
	{
		auto x = std::distance(Particles.begin(), Iter);
		auto NormalizeWith = 127.5f / WinSettings.CurrentGreatestVolicity;

		for (int Y = 0; Y < WinSettings.Info.MapY; ++Y)
		{
			Map[x][Y] = { 0,0 };
			for (auto& Item : Particles[x][Y])
			{
				Map[x][Y] += Item.Velocity;
			}

			
			unsigned tmpx = stde::Trim(127.5f - Map[x][Y].x*NormalizeWith*WinSettings.Contrast, 0, 255);
			unsigned tmpy = stde::Trim(127.5f - Map[x][Y].y*NormalizeWith*WinSettings.Contrast, 0, 255);

			//if (Map[x][y].x < 0) tmp = 0;

			sf::Color Color(
				std::abs(127.5f - tmpx),
				255 - std::abs(0.f + tmpy + tmpx),
				std::abs(127.5f - tmpy)
			);

			SpeedValueImage.setPixel(x, Y, Color);
		}
	});

	/*stde::For_each<NumberOfThreadsRunning>(Particles.begin(), Particles.end(), [&](decltype(Particles.begin()) Iter)
	{
		auto x = std::distance(Particles.begin(), Iter);

		for (int Y = 0; Y < WinSettings.Info.MapY; ++Y)
		{
			if (Map[x][Y].x == 0.f)
			{
				for (int Index = 1; Index < 2 && Y + Index < WinSettings.Info.MapY && Y - Index> 0; ++Index)
				{
					if (Map[x][Y + Index].x != 0.f)
					{
						Map[x][Y].x = Map[x][Y + Index].x /= 2;
					}
					else if (Map[x][Y - Index].x != 0.f)
					{
						Map[x][Y].x = Map[x][Y - Index].x /= 2;
					}
				}
			}


		}
	});*/

	//UpdateSpeedValueImage();

	

	SpeedValueTexture.setSmooth(true);


	SpeedValueTexture.loadFromImage(SpeedValueImage);

	SpeedValueSprite.setTexture(SpeedValueTexture, true);

	SpeedValueSprite.setScale(WinSettings.Info.Scale);

	Window.draw(SpeedValueSprite);
}

void FluidVelocityMap::CirclesRender(sf::RenderTarget & Window)
{
	for (int X = 0; X < WinSettings.Info.MapX; ++X)
		for (int Y = 0; Y < WinSettings.Info.MapY; ++Y)
			for (auto& P : Particles[X][Y])
				P.Draw(Window);
}

void FluidVelocityMap::UpdatePaths()
{
	stde::For_each<NumberOfThreadsRunning>(Map.begin(), Map.end(), [&](decltype(Map.begin()) Iter)
	{
		bool StartFounded{ false };
		unsigned Start{ 0 };

		auto x = std::distance(Map.begin(), Iter);

		auto& IsFieldOccupiedColumn = IsFieldOccupied[x];
		auto& NotOccupiedPathsList = NotOccupiedPaths[x];

		NotOccupiedPathsList.clear();

		for (unsigned y = 0; y < WinSettings.Info.MapY; ++y)
		{
			if (!StartFounded && !IsFieldOccupiedColumn[y])
			{
				Start = y;
				StartFounded = true;
			}
			else if (StartFounded && (IsFieldOccupiedColumn[y] || y == WinSettings.Info.MapY-1 ))
			{
				NotOccupiedPathsList.push_front(std::make_pair(Start, y+1));
				StartFounded = false;
			}
		}
	});		
}


void FluidVelocityMap::UpdateSpeedValueImage()
{
	SpeedValueImage.create(WinSettings.Info.MapX, WinSettings.Info.MapY, sf::Color::Black);

	stde::For_each<NumberOfThreadsRunning>(Map.begin(), Map.end(), [&](decltype(Map.begin()) Iter)
	{
		auto x = std::distance(Map.begin(), Iter);

		for (auto Boundry : NotOccupiedPaths[x])
			for (unsigned y = Boundry.first; y < Boundry.second; ++y)
				{			
					auto NormalizeWith = 127.5f / WinSettings.CurrentGreatestVolicity;
					unsigned tmpx = stde::Trim(127.5f - Map[x][y].x*NormalizeWith*WinSettings.Contrast, 0, 255);
					unsigned tmpy = stde::Trim(127.5f - Map[x][y].x*NormalizeWith*WinSettings.Contrast, 0, 255);

					//if (Map[x][y].x < 0) tmp = 0;

					sf::Color Color(
						std::abs(127.5f - tmpx),
						255 - std::abs(0.f + tmpy + tmpx),
						std::abs(127.5f - tmpy)
						);

					SpeedValueImage.setPixel(x, y, Color);
				}
	});
}

unsigned FluidVelocityMap::NotOccupiedFields(const unsigned Column)
{
	unsigned Res{0};

	for (auto Boundry : NotOccupiedPaths[Column])
		Res += Boundry.second - Boundry.first;

	return Res;
}

void FluidVelocityMap::Update(IObject *& State)
{
	if (State && State->IsStatic())
	{
		for (auto& Field : State->OccupiedFields)
			if (Field.Point.x < IsFieldOccupied.size() && Field.Point.y < IsFieldOccupied[Field.Point.x].size())
				IsFieldOccupiedStaticly[Field.Point.x][Field.Point.y] = false;

		for (auto& Object : WinSettings.Objects->Objects)
			if (Object.get() != State)
				for (auto& Field : Object->OccupiedFields)
					if (Field.Point.x < IsFieldOccupied.size() && Field.Point.y < IsFieldOccupied[Field.Point.x].size())
						IsFieldOccupiedStaticly[Field.Point.x][Field.Point.y] = true;

	}
}

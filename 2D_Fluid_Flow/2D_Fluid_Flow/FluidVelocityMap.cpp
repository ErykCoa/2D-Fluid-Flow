#include "FluidVelocityMap.h"


void FluidVelocityMap::FillUnoccupiedFields()
{
	std::normal_distribution<float> PositionDis{0.f, 0.2f};
	std::normal_distribution<float> VolacityDis{0.f, 0.05f};

	for (int X = 0; X < WinSettings.Info.MapX; X += WinSettings.ParticleRadius * 2.f)
		for (int Y = 0; Y < WinSettings.Info.MapY; Y += WinSettings.ParticleRadius * 2.f)
			if (!IsFieldOccupied[X][Y])
				Particles[X][Y].push_back(
					Spawner.SpawnNew(
						{ X + 0.5f, Y + 0.5f }, Boundaries<float>{ (float)X, X + 1.f, (float)Y, Y + 1.f }, PositionDis,
						{ 0.f, 0.f }, Boundaries<float>{ -0.01f, 0.01f, -0.01f, 0.01f }, VolacityDis
					)
				);					
}

void FluidVelocityMap::MovementTick()
{
	UpdatePaths();

	for (auto& Path : NotOccupiedPaths[0])
	{
		Spawner.AddNewSpawningBox(
			SpawningBox(sf::Rect<unsigned>(0, Path.first, 1, Path.second - Path.first - 2), 1.f, FlowInitialSpeed)
		);
	}

	MapBasicParticles();
}

void FluidVelocityMap::Draw(sf::RenderWindow & Window)
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
}


FluidVelocityMap::FluidVelocityMap(Settings & WinSettings) :
	FlowInitialSpeed{ 38.f, 0.f },
	WinSettings{ WinSettings },
	Particles( WinSettings.Info.MapX, std::vector<std::vector<Particle>>(WinSettings.Info.MapY) ),
	NextParticles(WinSettings.Info.MapX, std::vector<std::vector<Particle>>(WinSettings.Info.MapY)),
	NotOccupiedPaths(WinSettings.Info.MapX),
	IsFieldOccupied( WinSettings.Info.MapX, std::vector<bool>(WinSettings.Info.MapY) ),
	IsFieldOccupiedStaticly( WinSettings.Info.MapX, std::vector<bool>(WinSettings.Info.MapY) ),
	MutArray(WinSettings.Info.MapX),
	Map(WinSettings.Info.MapX, std::vector<sf::Vector2f>(WinSettings.Info.MapY)),
	Spawner(WinSettings)
{
	
}


void FluidVelocityMap::MapBasicParticles()
{
//	static unsigned long long Counter = 0;

	for (int i = 5; i > 0; --i)
	{
		Particle P = Spawner.SpawnNew();

		Particles[0][P.Position.x].push_back(P);
		
	}
	

	/*	if (Counter++ < 200)
	{

		std::normal_distribution<float> PositionDis{ 0.f, 0.2f };
		std::normal_distribution<float> VolacityDis{ 0.f, 0.05f };

		for (int Field = 0; Field < WinSettings.Info.MapY; Field+=2)
			if(!IsFieldOccupied[0][Field] && !IsFieldOccupied[1][Field + 1] && !IsFieldOccupied[1][Field - 1])
			{
				//if (!Particles[0][Field].size())
					//Particles[0][Field].push_back({WinSettings, {0.5f, Field + 0.5f}, FlowInitialSpeed });
				//	++WinSettings.ParticlesCounter;

					Particles[0][Field].push_back(
						std::move(Particle(
							WinSettings,
							{ 0.5f, Field + 0.5f }, 
							Boundaries<float>{ 0.f, 1.f, (float)Field, Field + 1.f }, PositionDis,
							FlowInitialSpeed, 
							Boundaries<float>{ FlowInitialSpeed.x * 0.7f, FlowInitialSpeed.x * 1.3f, FlowInitialSpeed.y * 0.7f, FlowInitialSpeed.y * 1.3f }, VolacityDis
						)));
				
			}
			


	}*/

	stde::For_each<NumberOfThreadsRunning>(Particles.begin(), Particles.end(), [&](decltype(Particles.begin()) Iter)
	{
		auto x = std::distance(Particles.begin(), Iter);

		for (int Y = 0; Y < WinSettings.Info.MapY; ++Y)
		{
			//if (IsFieldOccupied[x][Y]) continue; // +10%
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

void FluidVelocityMap::RegularRender(sf::RenderWindow & Window)
{
	stde::For_each<NumberOfThreadsRunning>(Particles.begin(), Particles.end(), [&](decltype(Particles.begin()) Iter)
	{
		auto x = std::distance(Particles.begin(), Iter);

		for (int Y = 0; Y < WinSettings.Info.MapY; ++Y)
		{
			Map[x][Y] = { 0,0 };
			for (auto& Item : Particles[x][Y])
			{
				Map[x][Y] += Item.Velocity;
			}
		}
	});

	stde::For_each<NumberOfThreadsRunning>(Particles.begin(), Particles.end(), [&](decltype(Particles.begin()) Iter)
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
	});

	UpdateSpeedValueImage();

	sf::Texture Texture{};

	Texture.setSmooth(true);


	Texture.loadFromImage(SpeedValueImage);

	SpeedValueSprite.setTexture(Texture, true);

	SpeedValueSprite.setScale(WinSettings.Info.Scale);

	Window.draw(SpeedValueSprite);
}

void FluidVelocityMap::CirclesRender(sf::RenderWindow & Window)
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
					unsigned tmp = stde::Trim(abs(Map[x][y].x)*WinSettings.Contrast,0,255);

					if (Map[x][y].x < 0) tmp = 0;

					sf::Color Color(
						tmp,
						0,
						255 - tmp
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

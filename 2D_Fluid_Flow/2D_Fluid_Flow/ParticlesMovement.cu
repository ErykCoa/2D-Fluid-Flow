#include "CudaFunctions.cuh"

__constant__ WindowInfo Info;

struct square
{
	__host__ __device__
		T operator()(const T& x) const
	{
		return x * x;
	}
};


void CudaFunctions::ParticlesMovement(
	Settings & WinSettings,
	std::vector<std::vector<std::vector<Particle>>> & Particles,
	std::vector<std::vector<std::vector<Particle>>> & NextParticles,
	std::vector<std::vector<bool>> & IsFieldOccupied
)
{
	cudaMemcpyToSymbol(&Info, &WinSettings.Info, sizeof(WindowInfo));

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



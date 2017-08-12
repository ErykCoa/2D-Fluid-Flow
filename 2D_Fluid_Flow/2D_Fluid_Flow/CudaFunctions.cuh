#include "thrust/device_vector.h"
#include "thrust/functional.h"
#include "thrust/device_vector.h"
#include "Particle.h"
#include <vector>
#include "include/cuda.h"
#include "include/cuda_runtime.h"
#include "Settings.h"
#include "WindowInfo.h"

namespace CudaFunctions
{
	void ParticlesMovement(
		Settings & WinSettings,
		std::vector<std::vector<std::vector<Particle>>> & Particles,
		std::vector<std::vector<std::vector<Particle>>> & NextParticles,
		std::vector<std::vector<bool>> & IsFieldOccupied
	);
}

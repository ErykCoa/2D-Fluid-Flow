#include "CommonFunctions.h"

/*Particle CreateParticle(Settings & WinSettings, sf::Vector2f StartingPosition, Boundaries<float>& PositionBoun, std::normal_distribution<float>& PositionDis, sf::Vector2f StartingVolacity, Boundaries<float>& VolacityBoun, std::normal_distribution<float>& VolacityDis)
{
	static std::default_random_engine Engine{};

	++WinSettings.ParticlesCounter;

	return std::move(Particle{
		WinSettings,
		stde::Trim(sf::Vector2f{ StartingPosition.x + PositionDis(Engine), StartingPosition.y + PositionDis(Engine) }, PositionBoun),
		stde::Trim(sf::Vector2f{ StartingVolacity.x + VolacityDis(Engine), StartingVolacity.y + VolacityDis(Engine) }, VolacityBoun)
	});
}*/

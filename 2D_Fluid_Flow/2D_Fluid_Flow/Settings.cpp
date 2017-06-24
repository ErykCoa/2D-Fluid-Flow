#include "Settings.h"

void Settings::LoadConfiguration(const std::string & Path)
{
	auto Config = stde::LoadDictionary<float>(Path + "/MapInfo.txt");

	BaseSpringinessForce = stde::GetValue(Config, "BaseSpringinessForce");
	ParticleRadius = stde::GetValue(Config, "ParticleRadius");
	Contrast = stde::GetValue(Config, "Contrast");

	Info.MapX = stde::GetValue(Config, "MapX");
	Info.MapY = stde::GetValue(Config, "MapY");

	UnitsPerSecond = stde::GetValue(Config, "UnitsPerSecond");

	Info.Scale = { stde::GetValue(Config, "ScaleX"), stde::GetValue(Config, "ScaleY") };
}

void Settings::UpdateTickDuration()
{
	CurrentTickDuration = ParticleRadius / NextGreatestVolicity / 3.f;
}

void Settings::UpdateGreatestVolicity()
{
	CurrentGreatestVolicity = NextGreatestVolicity;
}

void Settings::TrySetNextGreatestVolicity(float With)
{
	if (With > CurrentGreatestVolicity && With > NextGreatestVolicity)
		NextGreatestVolicity = With;
}

Settings::Settings(const std::string & Path) :
	Overlay{ Info },
	ParticlesCounter{ 0 },
	CurrentGreatestVolicity{ 1.f },
	NextGreatestVolicity{ 1.f },
	RenderType{ RenderType::Regular }
{
	LoadConfiguration(Path);
}

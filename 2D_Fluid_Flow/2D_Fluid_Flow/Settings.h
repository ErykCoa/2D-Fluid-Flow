#pragma once

#include <string>
#include "stde.h"
#include <atomic>
#include "WindowInfo.h"
#include "Overlay.h"
#include "ConsoleIO.h"
#include <mutex>
#include "StructsAndEnums.h"

class Settings
{
public:
	void LoadConfiguration(const std::string & Path);

	void UpdateTickDuration();
	void UpdateGreatestVolicity();
	void TrySetNextGreatestVolicity(float With);

	WindowInfo Info;
	Overlay Overlay;
	ConsoleIO Console;

	std::atomic<unsigned long long> ParticlesCounter;

	RenderType RenderType;

	float BaseSpringinessForce;
	float ParticleRadius;
	float UnitsPerSecond;
	float Contrast;

	float CurrentTickDuration;
	float CurrentGreatestVolicity;
	std::atomic<float> NextGreatestVolicity;

	Settings(const std::string & Path);
};


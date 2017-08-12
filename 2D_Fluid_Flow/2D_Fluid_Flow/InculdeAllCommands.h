#pragma once

#include "SFML/Graphics.hpp"
#include "StructsAndEnums.h"
#include "Settings.h"
#include <string>
#include "InputPreconditions.h"
#include "stde.h"
#include "CommandState.h"
#include <set>
#include <experimental/filesystem>


namespace Commands {
	CommandInfo FPSCounter(Settings& WinSettings);
	CommandInfo Help(Settings& WinSettings, std::map<std::string, CommandInfo> & Commands);
	CommandInfo OverlayStyle(Settings& WinSettings);
	CommandInfo OverlayVisability(Settings& WinSettings);
	CommandInfo ParticlesCounter(Settings& WinSettings);
	CommandInfo RenderStep(Settings& WinSettings);
	CommandInfo RenderType(Settings& WinSettings);
	CommandInfo SetContrast(Settings& WinSettings);
	CommandInfo LoadMap(Settings& WinSettings);
	CommandInfo SetScale(Settings& WinSettings);
	CommandInfo NewSpawnBox(Settings& WinSettings);
	CommandInfo DeleteObject(Settings& WinSettings);
	CommandInfo Cancel(Settings& WinSettings, std::set<CommandState>& PendingCommands);
	CommandInfo Select(Settings& WinSettings);
	CommandInfo ParticlesPerSecond(Settings& WinSettings);
	CommandInfo Print(Settings& WinSettings);
	CommandInfo FillUnoccupiedFields(Settings& WinSettings);
	CommandInfo Quit(Settings& WinSettings);
	CommandInfo Start(Settings& WinSettings);
	CommandInfo ListExamples(Settings& WinSettings);
}
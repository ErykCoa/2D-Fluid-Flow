#include "InculdeAllCommands.h"

CommandInfo Commands::ParticlesPerSecond(Settings& WinSettings) {

	return CommandInfo{

		CommandID::ParticlesPerSecond,

		[&](std::string Input, std::set<char> AdditionalAttributes) {

		if (std::atof(Input.c_str()) < 0.f)
			return CommandStatus::Failed;

		WinSettings.SpawningBoxes->SetMaxParticlesPerSecond(std::atof(Input.c_str()));

		return CommandStatus::Completed;
	},

	{
		std::string{} +
		"Sets max particles spawned per second.\n"

	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Value - Sets MaxParticlesPerSecond to Value. Value must be greater then or equal 0.\n\n"
	}

	};
}
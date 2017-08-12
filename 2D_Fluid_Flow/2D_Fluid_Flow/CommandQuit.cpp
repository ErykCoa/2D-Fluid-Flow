#include "InculdeAllCommands.h"

CommandInfo Commands::Quit(Settings& WinSettings) {

	return CommandInfo{

		CommandID::Quit,

		[&](std::string Input, std::set<char> AdditionalAttributes) {

		WinSettings.IsReadyForStart = true;

		switch (WinSettings.MapState.load())
		{
		case PipeMapState::LoadNewMap: WinSettings.MapState.store(PipeMapState::Quit); break;
		case PipeMapState::Running: WinSettings.MapState.store(PipeMapState::LoadNewMap); break;
		}

		return CommandStatus::Completed;
	},

	{
		std::string{} +
		"Quit from current simulation or closes program if no map is loaded.\n"

	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Nothing - default behavior.\n\n"
	}

	};
}

#include "InculdeAllCommands.h"

CommandInfo Commands::Start(Settings& WinSettings) {

	return CommandInfo{

		CommandID::Start,

		[&](std::string Input, std::set<char> AdditionalAttributes) {

		WinSettings.IsReadyForStart = true;

		return CommandStatus::Completed;
	},

	{
		std::string{} +
		"Starts the simulation.\n"

	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Nothing - default behavior.\n\n"
	}

	};
}

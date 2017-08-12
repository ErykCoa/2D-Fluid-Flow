#include "InculdeAllCommands.h"

CommandInfo Commands::Print(Settings& WinSettings) {

	return CommandInfo{

		CommandID::Print,

		[&](std::string Input, std::set<char> Attributes) {

		WinSettings.Console << Input + '\n';	

		return CommandStatus::Completed;
	},

	{
		std::string{} +
		"Prints message.\n"

	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"String - Prints String in Console.\n\n"
	}

	};
}

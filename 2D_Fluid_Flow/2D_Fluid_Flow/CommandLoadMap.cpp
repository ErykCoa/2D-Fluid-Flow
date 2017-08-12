#include "InculdeAllCommands.h"

CommandInfo Commands::LoadMap(Settings& WinSettings) {

	return CommandInfo{

		CommandID::LoadMap,

		[&](std::string Input, std::set<char> Attributes) {

		if (!WinSettings.TrySetMapPath(Input))
			WinSettings.Console << "Invalid MapPath.\n";

		for (char Attribute : Attributes)
			switch (Attribute)
			{
			default: WinSettings.Console << std::string("Invalid attribute (") + Attribute + "). Type \"help\" for more information.\n";
			}

		return CommandStatus::Completed;
	},

	{
		std::string{} +
		"Loads Map.\n"
	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Directory name - Loads Map from the directory.\n\n" 
	}

	};
}

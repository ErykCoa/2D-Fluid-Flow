#include "InculdeAllCommands.h"

CommandInfo Commands::RenderType(Settings& WinSettings) {

	return CommandInfo{

		CommandID::RenderType,

		[&](std::string Input, std::set<char> Attributes) {

		if (Input == "None")
		{
			WinSettings.RenderType = RenderType::None;
		}
		else if (Input == "Circles")
		{
			WinSettings.Console << "Not implemented yet.\n";
			//WinSettings.RenderType = RenderType::Circles;
		}
		else if (Input == "Regular")
		{
			WinSettings.RenderType = RenderType::Regular;
		}
		else
			WinSettings.Console << "Invalid value. Correct are: \"None/Circles/Regular\"\n";

		return CommandStatus::Completed;
	},

	{
		std::string{} +
		"Changes RenderType to value.\n"
	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"None - do not render particles.\n" +
		"Circles - render particles as circles.\n" +
		"Regular - render particles as bitmap.\n\n"
	}

	};
}

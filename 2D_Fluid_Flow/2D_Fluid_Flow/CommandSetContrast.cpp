#include "InculdeAllCommands.h"

CommandInfo Commands::SetContrast(Settings& WinSettings) {

	return CommandInfo{ 
		
		CommandID::SetContrast,

		[&](std::string Input, std::set<char> Attributes) {

		if (atof(Input.c_str()) != 0.f)
		{
			WinSettings.Contrast = atof(Input.c_str());
		}
		else
			WinSettings.Console << "Invalid value.\n";

		return CommandStatus::Completed;
	},

	{
		std::string{} +
		"Changes value of Contrast.\n"
	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Float number - change contrast to value.\n\n"
	}

	};
}

#include "InculdeAllCommands.h"

CommandInfo Commands::RenderStep(Settings& WinSettings) {

	return CommandInfo{

		CommandID::RenderStep,

		[&](std::string Input, std::set<char> Attributes) {

		if (atof(Input.c_str()) != 0.f)
		{
			WinSettings.TimeUnitsPerFrame = atof(Input.c_str());
		}
		else if (Attributes.find('e') == Attributes.end())
			WinSettings.Console << "Invalid value.\n";

		for (char Attribute : Attributes)
			switch (Attribute)
			{
			case 'e': WinSettings.TimeUnitsPerFrame = 0.f; break;
			default: WinSettings.Console << std::string("Invalid attribute (") + Attribute + "). Type \"help\" for more information.\n";
			}

		return CommandStatus::Completed;
	},

	{
		std::string{} +
		"Changes minimal RenderStep.\n"

	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Float number - change RenderStep to value.\n\n" +
		"Possible attributes:\n\n" +
		"e - render every frame.\n\n"
	}

	};
}

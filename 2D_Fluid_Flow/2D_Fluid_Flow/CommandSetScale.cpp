#include "InculdeAllCommands.h"

CommandInfo Commands::SetScale(Settings& WinSettings) {

	return CommandInfo{ 
		
		CommandID::SetScale,
		
		[&](std::string Input, std::set<char> Attributes) {

		if (atof(Input.c_str()) != 0.f)
		{
			sf::Vector2f NewScale{ static_cast<float>(atof(Input.c_str())) , static_cast<float>(atof(Input.c_str())) };
			
			if (std::abs(WinSettings.Info.Scale.x - NewScale.x) > 0.1)
				WinSettings.Info.Scale = NewScale;
		}
		else
			WinSettings.Console << "Invalid value.\n";

		return CommandStatus::Completed;
	},

	{
		std::string{} +
		"Changes value of Scale.\n"
	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Float number - change scale to value.\n\n"
	}

	};
}
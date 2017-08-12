#include "InculdeAllCommands.h"

CommandInfo Commands::OverlayVisability(Settings& WinSettings) {

	return CommandInfo{

		CommandID::OverlayVisability,

		[&](std::string Input, std::set<char> AdditionalAttributes) {

		if (Input == "")
		{
			WinSettings.Overlay.SetVisability(!WinSettings.Overlay.GetVisability());
		}
		else if (Input == "1" || Input == "set" || Input == "on")
		{
			WinSettings.Overlay.SetVisability(true);
		}
		else if (Input == "0" || Input == "off")
		{
			WinSettings.Overlay.SetVisability(false);
		}
		else
			WinSettings.Console << "Invalid value. Correct are: \"1/set/on/0/off\"\n";

		return CommandStatus::Completed;
	},

	{
		std::string{} +
		"Changes Overlay visability.\n"

	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Nothing - switch Overlay visability.\n" +
		"1/on/set - change Overlay visability to 1.\n" +
		"0/off - change Overlay visability to 0.\n\n"
	}
	};
}

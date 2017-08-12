#include "InculdeAllCommands.h"

CommandInfo Commands::FPSCounter(Settings& WinSettings) {

	return CommandInfo{

		CommandID::FPSCounter,

		[&](std::string Input, std::set<char> AdditionalAttributes) {

		if (Input == "")
		{
			if (WinSettings.Overlay.HasItem("FPS"))
				WinSettings.Overlay.RemoveItem("FPS");
			else
				WinSettings.Overlay.AddItem("FPS", [&] { return WinSettings.Info.FPS.GetString(); });
		}
		else if (Input == "1" || Input == "set" || Input == "on")
		{
			WinSettings.Overlay.AddItem("FPS", [&] { return WinSettings.Info.FPS.GetString(); });
		}
		else if (Input == "0" || Input == "off")
		{
			WinSettings.Overlay.RemoveItem("FPS");
		}
		else
			WinSettings.Console << "Invalid value. Correct are: \"1/set/on/0/off\"\n";

		return CommandStatus::Completed;
	},

	{
		std::string{} +
		"Changes FPS Counter visability.\n"

	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Nothing - switch FPS Counter visability.\n" +
		"1/on/set - change FPS Counter visability to 1.\n" +
		"0/off - change FPS Counter visability to 0.\n\n"
	}

	};
}


#include "InculdeAllCommands.h"

CommandInfo Commands::ParticlesCounter(Settings& WinSettings) {

	return CommandInfo{

		CommandID::ParticlesCounter,

		[&](std::string Input, std::set<char> Attributes) {

		if (Input == "")
		{
			if (WinSettings.Overlay.HasItem("Particles"))
				WinSettings.Overlay.RemoveItem("Particles");
			else
				WinSettings.Overlay.AddItem("Particles", [&] { return std::to_string(WinSettings.ParticlesCounter.load()); });

		}
		else if (Input == "1" || Input == "set" || Input == "on")
		{
			WinSettings.Overlay.AddItem("Particles", [&] { return WinSettings.Info.FPS.GetString(); });
		}
		else if (Input == "0" || Input == "off")
		{
			WinSettings.Overlay.RemoveItem("Particles");
		}
		else
			WinSettings.Console << "Invalid value. Correct are: \"1/set/on/0/off\"\n";

		return CommandStatus::Completed;
	},

	{
		std::string{} +
		"Changes Particles Counter visability.\n"

	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Nothing - switch Particles Counter visability.\n" +
		"1/on/set - change Particles Counter visability to 1.\n" +
		"0/off - change Particles Counter visability to 0.\n\n"
	}

	};
}

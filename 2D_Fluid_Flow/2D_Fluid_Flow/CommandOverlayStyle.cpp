#include "InculdeAllCommands.h"

CommandInfo Commands::OverlayStyle(Settings& WinSettings) {

	return CommandInfo{

		CommandID::OverlayStyle,

		[&](std::string Input, std::set<char> AdditionalAttributes) {

		if (Input == "1" || Input == "on")
		{
		}
		else if (Input == "0" || Input == "set" || Input == "off" || Input == "")
		{
			WinSettings.Overlay.SetFontStyle(sf::Text::Regular);
		}
		else
			WinSettings.Console << "Invalid value. Correct are: \"1/set/on/0/off\"\n";

		for (char Attribute : AdditionalAttributes)
			switch (Attribute)
			{
			case 'b': WinSettings.Overlay.ChangeFontStyle(sf::Text::Bold); break;
			case 'i': WinSettings.Overlay.ChangeFontStyle(sf::Text::Italic); break;
			case 'u': WinSettings.Overlay.ChangeFontStyle(sf::Text::Underlined); break;
			default: WinSettings.Console << std::string("Invalid attribute (") + Attribute + "). Type \"help\" for more information.\n";
			}

		return CommandStatus::Completed;
	},

	{
		std::string{} +
		"Changes Overlay style.\n"

	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"1/on - switch Overlay styles specified as attributes.\n" +
		"0/off/set - reset current and set Overlay styles specified as attributes.\n\n" +
		"Possible attributes:\n\n" +
		"b - bold.\n" +
		"i - italic.\n" +
		"u - underlined.\n\n"
	}
	};
}

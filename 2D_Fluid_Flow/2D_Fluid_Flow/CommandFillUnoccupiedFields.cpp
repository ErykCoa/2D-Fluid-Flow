#include "InculdeAllCommands.h"

CommandInfo Commands::FillUnoccupiedFields(Settings& WinSettings) {

	return CommandInfo{

		CommandID::FillUnoccupiedFields,

		[&](std::string Input, std::set<char> AdditionalAttributes) {

		WinSettings.FluidMap->FillUnoccupiedFields();

		return CommandStatus::Completed;
	},

	{
		std::string{} +
		"Fills currently not occupied, by static or dynamic objects, fields.\n"
	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Nothing - default behavior.\n\n"
	}

	};
}
#include "InculdeAllCommands.h"

CommandInfo Commands::Select(Settings& WinSettings) {

	return CommandInfo{

		CommandID::Select,

		[&](std::string Input, std::set<char> AdditionalAttributes) {

		if (!WinSettings.Objects->IsAnySelected())
		{
			WinSettings.Objects->AwaitSelection();
			WinSettings.Objects->SetVisability(VisabilityType::Visible);
			return CommandStatus::Pending;
		}

		return CommandStatus::Completed;
	},

	{
		std::string{} +
		"Awaits selection of object.\n"

	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Nothing - default behavior.\n\n"
	}

	};
}

#include "InculdeAllCommands.h"

CommandInfo Commands::DeleteObject(Settings& WinSettings) {

	return CommandInfo{

		CommandID::DeleteObject,

		[&](std::string Input, std::set<char> AdditionalAttributes) {

		if (!WinSettings.Objects->IsAnySelected())
		{
			WinSettings.Objects->AwaitSelection();
			WinSettings.Objects->SetVisability(VisabilityType::Visible);
			return CommandStatus::Pending;
		}

		WinSettings.Objects->SetVisability(VisabilityType::Default);
		WinSettings.Objects->DeleteSelected();

		return CommandStatus::Completed;
	},

	{
		std::string{} +
		"Deletes selected object.\n"
	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Nothing - Deletes selected object. If none is selected awaits selection.\n\n"
	}

	};
}

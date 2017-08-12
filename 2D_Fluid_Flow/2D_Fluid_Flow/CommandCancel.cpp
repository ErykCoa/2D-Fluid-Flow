#include "InculdeAllCommands.h"

CommandInfo Commands::Cancel(Settings& WinSettings, std::set<CommandState>& PendingCommands) {

	return CommandInfo{

		CommandID::Cancel,

		[&](std::string Input, std::set<char> AdditionalAttributes) {

		for (auto& Command : PendingCommands)
		{
			WinSettings.Console << "Command " + Command.Name + " has been canceled.\n";
		}

		PendingCommands.clear();

		WinSettings.Objects->CancelSelection();
		WinSettings.Objects->SetVisability(VisabilityType::Default);

		WinSettings.SelectedWindowArea.Cancel();

		return CommandStatus::Completed;
	},

	{
		std::string{} +
		"Cancels all pending commands and objects selection.\n"

	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Nothing - default behavior.\n\n" 
	}

	};
}

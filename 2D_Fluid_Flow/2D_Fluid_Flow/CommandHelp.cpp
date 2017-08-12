#include "InculdeAllCommands.h"

CommandInfo Commands::Help(Settings& WinSettings, std::map<std::string, CommandInfo> & Commands) {
	
	return CommandInfo{

		CommandID::Help,

		[&](std::string Input, std::set<char> Attributes) {

		if (Input == "")
		{
			std::map<CommandInfo, std::string> CommandsToName{};

			for (auto& Cmd : Commands)
			{
				if (CommandsToName.find(Cmd.second) == CommandsToName.end())
					CommandsToName[Cmd.second] = Cmd.first;
				else 
					CommandsToName[Cmd.second] += ", " + Cmd.first;
			}

			WinSettings.Console << "\nCommands:\n";

			for (auto& Cmd : CommandsToName)
			{
				WinSettings.Console << "\t" + Cmd.second + " - " + Cmd.first.Help;
			}

			WinSettings.Console << "\n";

		}
		else if (Commands.find(Input) != Commands.end())
		{
			auto Cmd = Commands.find(Input);
			WinSettings.Console << "\n" + Cmd->first + " - " + Cmd->second.Help + "\n" + Cmd->second.MoreHelp;
		}
		else
			WinSettings.Console << "Invalid value. Command: " + Input + " does not exist.\n";

		return CommandStatus::Completed;
	},

	{
		std::string{} +
		"Shows list od commands or, if value is given, shows advanced help for command.\n"
	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Command Name - shows advanced help for command.\n\n"
	}

	};
}

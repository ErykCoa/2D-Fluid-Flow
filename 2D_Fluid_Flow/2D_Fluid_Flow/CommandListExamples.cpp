#include "InculdeAllCommands.h"

CommandInfo Commands::ListExamples(Settings& WinSettings) {

	return CommandInfo{

		CommandID::ListExamples,

		[&](std::string Input, std::set<char> AdditionalAttributes) {

		WinSettings.Console << "\n";

		for (auto& Path : std::experimental::filesystem::directory_iterator("."))
		{
			std::string PathName = Path.path().string();
			if ((PathName.find("example") != PathName.npos || PathName.find("Example") != PathName.npos) && std::experimental::filesystem::is_directory(Path.path()))
				WinSettings.Console << '\t' + PathName + '\n';
		}

		WinSettings.Console << "\n";

		return CommandStatus::Completed;
	},

	{
		std::string{} +
		"Lists all available examples.\n"

	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Nothing - default behavior.\n\n"
	}

	};
}

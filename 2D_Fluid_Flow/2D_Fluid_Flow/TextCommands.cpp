#include "TextCommands.h"

void TextCommands::ProcessCommands()
{
	ExecutePendingCommands();

	for (auto& CmdStr : WinSettings.Console.PopInputLines())
	{
		WinSettings.Console << ">> " + CmdStr + '\n';

		ProcessCommand(CmdStr);
	}
}

void TextCommands::ProcessCommand(std::string & Command)
{
	std::string CmdName{};
	std::set<char> Attributes{};
	std::string Value{};

	std::tie(CmdName, Attributes, Value) = Tokenize(Command);

	Process(CmdName, Attributes, Value);
}

void TextCommands::Process(std::string & Command, std::set<char> & Attributes, std::string & Value)
{
	auto Com = Commands.find(Command);

	if (Com != Commands.end())
	{
		switch (Com->second.Function(Value, Attributes))
		{
		case CommandStatus::Failed: CommandHasFailed({Command, Value, Attributes, Com->second }); break;
		case CommandStatus::Pending: PendingCommands.insert({Command, Value, Attributes, Com->second }); break;
		default: break;
		}
	}
	else
		WinSettings.Console << "Command does not exist. Type \"help\" for more information.\n";
}

std::tuple<std::string, std::set<char>, std::string> TextCommands::Tokenize(std::string & Command)
{
	std::string CmdName{};
	std::set<char> Attributes{};
	std::string Value{};

	for (auto Iter = Command.begin(); Iter != Command.end(); ++Iter)
	{
		if (*Iter == ' ' || *Iter == '\t')
			continue;

		if (CmdName.empty())
		{
			CmdName = GetString(Iter, Command);
		}
		else if (*Iter == '-')
		{
			++Iter;
			for (char Atr : GetString(Iter, Command))
				Attributes.insert(Atr);
		}
		else if (Value.empty())
		{
			Value = GetString(Iter, Command);
		}


		if (Iter == Command.end()) break;
	}

	return{ CmdName, Attributes, Value };
}

void TextCommands::SetCommands(CommandsSet Commands)
{
	switch (Commands)
	{
	case CommandsSet::MapLoading: SetMapLoadingCommands(); break;
	case CommandsSet::Default: SetDefaultCommands(); break;
	default:break;
	}
}

TextCommands::TextCommands(Settings & WinSettings, CommandsSet Commands) :
	WinSettings(WinSettings)
{
	SetCommands(Commands);
}


void TextCommands::SetDefaultCommands()
{
	Commands["OverlayVisability"] = Commands["Overlay"] = Commands::OverlayVisability(WinSettings);

	Commands["OverlayStyle"] = Commands["TextStyle"] = Commands::OverlayStyle(WinSettings);

	Commands["FPSCounter"] = Commands["FPS"] = Commands::FPSCounter(WinSettings);

	Commands["ParticlesCounter"] = Commands["ParticlesCount"] = Commands::ParticlesCounter(WinSettings);

	Commands["help"] = Commands::Help(WinSettings, Commands);

	Commands["RenderType"] = Commands["Render"] = Commands::RenderType(WinSettings);

	Commands["Contrast"] = Commands["SetContrast"] = Commands::SetContrast(WinSettings);

	Commands["Step"] = Commands["RenderStep"] = Commands::RenderStep(WinSettings);

	Commands["Scale"] = Commands["SetScale"] = Commands::SetScale(WinSettings);

	Commands["AddSpawner"] = Commands["NewSpawner"] = Commands::NewSpawnBox(WinSettings);

	Commands["Delete"] = Commands["DeleteObject"] = Commands::DeleteObject(WinSettings);

	Commands["Cancel"] = Commands::Cancel(WinSettings, PendingCommands);

	Commands["Select"] = Commands::Select(WinSettings);

	Commands["ParticlesPerSecond"] = Commands["PerSec"] = Commands::ParticlesPerSecond(WinSettings);

	Commands["Print"] = Commands::Print(WinSettings);

	Commands["FillUnoccupiedFields"] = Commands["FillMap"] = Commands::FillUnoccupiedFields(WinSettings);

	Commands["Quit"] = Commands["Return"] = Commands::Quit(WinSettings);

	Commands["Start"] = Commands::Start(WinSettings);
}

void TextCommands::SetMapLoadingCommands()
{
	Commands["LoadMap"] = Commands["Load"] = Commands::LoadMap(WinSettings);
	Commands["help"] = Commands::Help(WinSettings, Commands);
	Commands["Quit"] = Commands::Quit(WinSettings);
	Commands["ListExamples"] = Commands::ListExamples(WinSettings);
}

void TextCommands::ExecutePendingCommands()
{
	for (auto Cmd = PendingCommands.begin(); Cmd != PendingCommands.end(); ++Cmd)
	{
		switch (Cmd->Command.Function(Cmd->Input, Cmd->Attributes))
		{
		case CommandStatus::Failed: CommandHasFailed(*Cmd);
		case CommandStatus::Completed: PendingCommands.erase(Cmd);
		default: break;
		}
	}
}

void TextCommands::CommandHasFailed(const CommandState & State)
{
	WinSettings.Console << "Command " + State.Name + " has failed. Type \"help " + State.Name + "\" for more information.\n";
}

std::string TextCommands::GetString(decltype(std::string{}.begin()) & Iter, std::string & From)
{
	std::string Res{};
	char StartsWithQuote(0);
	bool IsNextEscaped(false);

	while (Iter != From.end() && ( StartsWithQuote || (*Iter != ' ' && *Iter != '\t')))
	{
		if (*Iter == '\'' || *Iter == '"')
		{
			if (!StartsWithQuote)
				StartsWithQuote = *Iter;
			else if (!IsNextEscaped && StartsWithQuote == *Iter)
			{
				++Iter;
				return std::move(Res);
			}
			IsNextEscaped = false;
		}
		else if (!IsNextEscaped && *Iter == '\\')
			IsNextEscaped = true;
		else if (IsNextEscaped)
		{
			if (*Iter == 'n')
				Res += '\n';
			else
				Res += *Iter;

			IsNextEscaped = false;
		}
		else 
			Res += *Iter;

		++Iter;
	}

	return std::move(Res);
}

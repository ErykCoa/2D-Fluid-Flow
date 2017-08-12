#pragma once
#include <map>
#include "StructsAndEnums.h"
#include "Settings.h"
#include <iostream>
#include <vector>
#include <set>
#include <cstring>
#include "InculdeAllCommands.h"
#include "CommandState.h"

class TextCommands
{
public:
	void ProcessCommands();
	void ProcessCommand(std::string & Command);

	TextCommands(Settings & WinSettings, CommandsSet Commands);

private:
	void Process(std::string & Command, std::set<char> & Attributes, std::string & Value);

	std::tuple<std::string, std::set<char>, std::string> Tokenize(std::string & Command);

	void SetCommands(CommandsSet Commands);

	void SetDefaultCommands();
	void SetMapLoadingCommands();

	void ExecutePendingCommands();

	void CommandHasFailed(const CommandState & State);

	std::string GetString(decltype(std::string{}.begin()) & Iter, std::string & From);

	Settings & WinSettings;
	std::map<std::string, CommandInfo> Commands;

	std::set<CommandState> PendingCommands;
};



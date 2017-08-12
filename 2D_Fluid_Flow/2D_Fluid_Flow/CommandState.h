#pragma once
#include "StructsAndEnums.h"

struct CommandState
{
	std::string Name;
	std::string Input;
	std::set<char> Attributes;
	CommandInfo Command;

	bool operator<(const CommandState & With) const
	{
		return Command < With.Command;
	}
};
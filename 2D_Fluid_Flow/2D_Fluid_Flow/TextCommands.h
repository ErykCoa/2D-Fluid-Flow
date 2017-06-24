#pragma once
#include <map>
#include "StructsAndEnums.h"
#include "Settings.h"
#include <iostream>
#include <vector>
#include <cstring>

class TextCommands
{
public:
	void ProcessCommands();

	TextCommands(Settings & WinSettings);

private:
	void Process(std::string & Command, std::string & Attributes, std::string & Value);

	void SetDefaultCommands();

	std::string GetString(decltype(std::string{}.begin()) & Iter, std::string & From);

	Settings & WinSettings;
	std::map<std::string, CommandInfo> Commands;
};



#include "TextCommands.h"

void TextCommands::ProcessCommands()
{
	for (auto& CmdStr : WinSettings.Console.PopInputLines())
	{
		WinSettings.Console << ">> " + CmdStr + '\n';

		std::string CmdName{};
		std::string Attributes{};
		std::string Value{};

		bool IsCommandValid{ true };

		for (auto Iter = CmdStr.begin(); Iter != CmdStr.end(); ++Iter)
		{
			if (*Iter == ' ' || *Iter == '\t') 
				continue;

			if (CmdName.empty())
			{
				CmdName = GetString(Iter, CmdStr);
			}
			else if (*Iter == '-')
			{
				++Iter;
				Attributes = GetString(Iter, CmdStr);
			}
			else if (Value.empty())
			{
				Value = GetString(Iter, CmdStr);
			}
			else
			{
				WinSettings.Console << "Invalid command: " + CmdStr + '\n';
				IsCommandValid = false;
			}

			if (Iter == CmdStr.end()) break;
		}

		if (IsCommandValid)
			Process(CmdName, Attributes, Value);
	}


}

void TextCommands::Process(std::string & Command, std::string & Attributes, std::string & Value)
{
	auto Com = Commands.find(Command);

	if (Com != Commands.end())
	{
		for (auto& Atr : Attributes)
			if (Com->second.AdditionalParametrs.find(Atr) == Com->second.AdditionalParametrs.end())
			{
				WinSettings.Console << std::string("Invalid attribute (") + Atr + "). Type \"help\" for more information.\n";
				return;
			}

		Com->second.Function(Value);
		for (auto& Atr : Attributes)
			Com->second.AdditionalParametrs[Atr]();

	}
	else
		WinSettings.Console << "Command does not exist. Type \"help\" for more information.\n";
}

TextCommands::TextCommands(Settings & WinSettings) :
	WinSettings(WinSettings)
{
	SetDefaultCommands();
}


void TextCommands::SetDefaultCommands()
{
	Commands["OverlayVisability"] = Commands["Overlay"] = {

		[&](std::string Input) {
		if (Input == "")
		{
			WinSettings.Overlay.SetVisability(!WinSettings.Overlay.GetVisability());
		}
		else if (Input == "1" || Input == "set" || Input == "on")
		{
			WinSettings.Overlay.SetVisability(true);
		}
		else if (Input == "0" || Input == "off")
		{
			WinSettings.Overlay.SetVisability(false);
		}
		else
			WinSettings.Console << "Invalid value. Correct are: \"1/set/on/0/off\"\n";
	},

	{},

	{
		std::string{} +
		"Changes Overlay visability.\n"

	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Nothing - switch Overlay visability.\n" +
		"1/on/set - change Overlay visability to 1.\n" +
		"0/off - change Overlay visability to 0.\n\n"
	}
	};

	Commands["OverlayStyle"] = Commands["TextStyle"] = {

		[&](std::string Input) {

		if (Input == "1" || Input == "on")
		{
		}
		else if (Input == "0" || Input == "set" || Input == "off" || Input == "")
		{
			WinSettings.Overlay.SetFontStyle(sf::Text::Regular);
		}
		else
			WinSettings.Console << "Invalid value. Correct are: \"1/set/on/0/off\"\n";
	},

		std::map<char, std::function<void()>>{
			{'b', [&] { WinSettings.Overlay.ChangeFontStyle(sf::Text::Bold); }},
			{'i', [&] { WinSettings.Overlay.ChangeFontStyle(sf::Text::Italic); }},
			{'u', [&] { WinSettings.Overlay.ChangeFontStyle(sf::Text::Underlined); }}
	},

	{
		std::string{} +
		"Changes Overlay style.\n"

	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"1/on - switch Overlay styles specified as attributes.\n" +
		"0/off/set - reset current and set Overlay styles specified as attributes.\n\n" +
		"Possible attributes:\n\n" +
		"b - bold.\n" +
		"i - italic.\n" +
		"u - underlined.\n\n"
	}

	};

	Commands["FPSCounter"] = Commands["FPS"] = {

		[&](std::string Input) {

		if (Input == "")
		{
			if (WinSettings.Overlay.HasItem("FPS"))
				WinSettings.Overlay.RemoveItem("FPS");
			else 
				WinSettings.Overlay.AddItem("FPS", [&] { return WinSettings.Info.FPS.GetString(); });
		}
		else if (Input == "1" || Input == "set" || Input == "on")
		{
			WinSettings.Overlay.AddItem("FPS", [&] { return WinSettings.Info.FPS.GetString(); });
		}
		else if (Input == "0" || Input == "off")
		{
			WinSettings.Overlay.RemoveItem("FPS");
		}
		else 
			WinSettings.Console << "Invalid value. Correct are: \"1/set/on/0/off\"\n";
	},

	{},

	{
		std::string{} +
		"Changes FPS Counter visability.\n"

	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Nothing - switch FPS Counter visability.\n" +
		"1/on/set - change FPS Counter visability to 1.\n" +
		"0/off - change FPS Counter visability to 0.\n\n"
	}

	};

	Commands["ParticlesCounter"] = Commands["ParticlesCount"] = {

		[&](std::string Input) {

		if (Input == "")
		{
			if (WinSettings.Overlay.HasItem("Particles"))
				WinSettings.Overlay.RemoveItem("Particles");
			else
			{
				
				WinSettings.Overlay.AddItem("Particles", [&] { return std::to_string(WinSettings.ParticlesCounter); });
			}
				
		}
		else if (Input == "1" || Input == "set" || Input == "on")
		{
			WinSettings.Overlay.AddItem("Particles", [&] { return WinSettings.Info.FPS.GetString(); });
		}
		else if (Input == "0" || Input == "off")
		{
			WinSettings.Overlay.RemoveItem("Particles");
		}
		else
			WinSettings.Console << "Invalid value. Correct are: \"1/set/on/0/off\"\n";
	},

	{},

	{
		std::string{} +
		"Changes Particles Counter visability.\n"

	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Nothing - switch Particles Counter visability.\n" +
		"1/on/set - change Particles Counter visability to 1.\n" +
		"0/off - change Particles Counter visability to 0.\n\n"
	}

	};

	Commands["help"] = {

		[&](std::string Input) {

		if (Input == "")
		{
			WinSettings.Console << "\nCommands:\n";

			for (auto& Cmd : Commands)
			{
				WinSettings.Console << "\t" + Cmd.first + " - " + Cmd.second.Help;
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
	},

	{},

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

	Commands["RenderType"] = Commands["Render"] = {

		[&](std::string Input) {

		if (Input == "None")
		{
			WinSettings.RenderType = RenderType::None;
		}
		else if (Input == "Circles")
		{
			WinSettings.RenderType = RenderType::Circles;
		}
		else if (Input == "Regular")
		{
			WinSettings.RenderType = RenderType::Regular;
		}
		else
			WinSettings.Console << "Invalid value. Correct are: \"None/Circles/Regular\"\n";
	},

	{},

	{
		std::string{} +
		"Changes RenderType to value.\n"
	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"None - do not render particles.\n" +
		"Circles - render particles as circles.\n" +
		"Regular - render particles as bitmap.\n\n"
	}

	};


	Commands["Contrast"] = Commands["SetContrast"] = {

		[&](std::string Input) {

		if (atof(Input.c_str()) != 0.f)
		{
			WinSettings.Contrast = atof(Input.c_str());
		}
		else
			WinSettings.Console << "Invalid value.\n";
	},

	{},

	{
		std::string{} +
		"Changes value of Contrast.\n"
	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Float number - change contrast to value.\n\n" 
	}

	};

	Commands["Step"] = Commands["RenderStep"] = {

		[&](std::string Input) {

		if (atof(Input.c_str()) != 0.f)
		{
			WinSettings.UnitsPerSecond = atof(Input.c_str());
		}
		else
			WinSettings.Console << "Invalid value.\n";
	},

		std::map<char, std::function<void()>>{
			{'e', [&] { WinSettings.UnitsPerSecond = 0.f; }}
	},

	{
		std::string{} +
		"Changes minimal RenderStep.\n"

	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Float number - change RenderStep to value.\n\n" +
		"Possible attributes:\n\n" +
		"e - render every frame.\n\n"
	}

	};

}

std::string TextCommands::GetString(decltype(std::string{}.begin()) & Iter, std::string & From)
{
	std::string Res{};

	while (Iter != From.end() && *Iter != ' ' && *Iter != '\t')
	{
		Res += *Iter;
		++Iter;
	}

	return std::move(Res);
}

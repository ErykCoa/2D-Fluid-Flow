#include "InculdeAllCommands.h"

CommandInfo Commands::NewSpawnBox(Settings& WinSettings) {

	return CommandInfo{

		CommandID::NewSpawnBox,

		[&](std::string Input, std::set<char> AdditionalAttributes) {

		auto InputValues = stde::Tokenize(Input);

		if ((InputValues.size() != 3 && InputValues.size() != 7) || std::atof(InputValues[0].c_str()) == 0.f)
			return CommandStatus::Failed;

		if (InputValues.size() == 7)
		{
			auto LeftX = std::atof(InputValues[3].c_str());
			auto LeftY = std::atof(InputValues[4].c_str());
			auto Width = std::atof(InputValues[5].c_str());
			auto Height = std::atof(InputValues[6].c_str());

			if (LeftX >= WinSettings.Info.MapX || LeftY >= WinSettings.Info.MapY)
				return CommandStatus::Failed;

			Width = stde::Trim(Width, 0, WinSettings.Info.MapX - LeftX);
			Height = stde::Trim(Height, 0, WinSettings.Info.MapY - LeftY);

			WinSettings.SpawningBoxes->AddNewSpawningBox(SpawningBox(
				sf::Rect<unsigned>(LeftX, LeftY, Width, Height),
				static_cast<float>(std::atof(InputValues[0].c_str())),
				sf::Vector2f(static_cast<float>(std::atof(InputValues[1].c_str())), static_cast<float>(std::atof(InputValues[2].c_str())))
			));

			return CommandStatus::Completed;
		}

		if ( !WinSettings.SelectedWindowArea.IsReady() )
			return CommandStatus::Pending;

		
		auto Field = WinSettings.SelectedWindowArea.Accept();
		
		//std::atof(Input.substr(Input.find_first_of(',')).c_str());

		WinSettings.SpawningBoxes->AddNewSpawningBox(SpawningBox(
			Field,
			static_cast<float>(std::atof(InputValues[0].c_str())),
			sf::Vector2f( static_cast<float>(std::atof(InputValues[1].c_str())), static_cast<float>(std::atof(InputValues[2].c_str())) )
		));

		return CommandStatus::Completed;
	},

	{
		std::string{} +
		"Adds particles spawning box.\n"

	},

	{
		std::string{} +
		"Possible values:\n\n" +
		"Thickness,X_Velocity,Y_Velocity - Creates spawning box within selected area with given parameters. Awaits selection if no area is selected.\n\n"
	}
	};
}
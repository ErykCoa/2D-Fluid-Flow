#include "Settings.h"


void Settings::LoadConfiguration()
{
 // TODO: if (MapPath.empty())

	auto Config = stde::LoadDictionary<float>(MapPath + "/MapInfo.txt");

	BaseSpringinessForce = stde::GetValue(Config, "BaseSpringinessForce");
	ParticleRadius = stde::GetValue(Config, "ParticleRadius");
	Contrast = stde::GetValue(Config, "Contrast");

	Info.MapX = stde::GetValue(Config, "MapX");
	Info.MapY = stde::GetValue(Config, "MapY");

	Info.LeftBorder = (stde::GetValue(Config, "IsLeftBorderBouncy") == 0.f) ? BorderType::Normal: BorderType::Bouncy;
	Info.RightBorder = (stde::GetValue(Config, "IsRightBorderBouncy") == 0.f) ? BorderType::Normal : BorderType::Bouncy;
	Info.TopBorder = (stde::GetValue(Config, "IsTopBorderBouncy") == 0.f) ? BorderType::Normal : BorderType::Bouncy;
	Info.BottomBorder = (stde::GetValue(Config, "IsBottomBorderBouncy") == 0.f) ? BorderType::Normal : BorderType::Bouncy;

	TimeUnitsPerFrame = stde::GetValue(Config, "TimeUnitsPerFrame");

	Info.Scale = { stde::GetValue(Config, "ScaleX"), stde::GetValue(Config, "ScaleY") };
}

std::string & Settings::GetMapPath()
{
	return MapPath;
}

bool Settings::IsMapPathSet()
{
	return !MapPath.empty();
}

bool Settings::TrySetMapPath(const std::string & With)
{
	if (IsPathCorrect(With))
	{
		MapPath = With;
		return true;
	}

	return false;
}

void Settings::UpdateTickDuration()
{
	CurrentTickDuration = ParticleRadius / NextGreatestVolicity / 3.f;
}

void Settings::UpdateGreatestVolicity()
{
	CurrentGreatestVolicity = NextGreatestVolicity;
}

void Settings::TrySetNextGreatestVolicity(float With)
{
	if (With > CurrentGreatestVolicity && With > NextGreatestVolicity)
		NextGreatestVolicity = With;
}


Settings::Settings(const std::string & Path, sf::RenderWindow & Window) :
	Overlay{ Info, ViewChanger },
	ParticlesCounter{ 0 },
	CurrentGreatestVolicity{ 1.f },
	NextGreatestVolicity{ 1.f },
	RenderType{ RenderType::Regular },
	MousePosition{ Window },
	SelectedWindowArea{ Overlay, Info, MousePosition },
	ViewChanger{Info, MousePosition},
	MapState{PipeMapState::Running}
{
	if (TrySetMapPath(Path))
		LoadConfiguration();

	std::unique_ptr<ObjectsHandler> Tmp{ new ObjectsHandler(MousePosition) };

	WindowEvents.EventSubject.Attach(*Tmp.get());

	Objects = std::move(Tmp);

	WindowEvents.EventSubject.Attach(MousePosition);
	WindowEvents.EventSubject.Attach(SelectedWindowArea);
	WindowEvents.EventSubject.Attach(ViewChanger);
}

Settings::Settings(sf::RenderWindow & Window) :
	Overlay{ Info, ViewChanger },
	ParticlesCounter{ 0 },
	CurrentGreatestVolicity{ 1.f },
	NextGreatestVolicity{ 1.f },
	RenderType{ RenderType::Regular },
	MousePosition{ Window },
	SelectedWindowArea{ Overlay, Info, MousePosition },
	ViewChanger{ Info, MousePosition },
	MapState{ PipeMapState::LoadNewMap }
{
}

bool Settings::IsPathCorrect(const std::string & MapPath)
{
	const static std::array<const char*, 2> NecessaryFiles{
		"/MapInfo.txt",
		"/Objects.txt"
	};

	for (auto FileName : NecessaryFiles)
		if (!std::experimental::filesystem::exists(MapPath + FileName)) return false;

	return true;
}
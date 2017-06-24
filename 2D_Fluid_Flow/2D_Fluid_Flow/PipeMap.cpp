#include "PipeMap.h"

void PipeMap::SingleTick()
{
	MovementTick();
	Draw();

	sf::Event event;
	while (Window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			Window.close();
	}

	CommandsProcessing.ProcessCommands();
}

bool PipeMap::WindowHasBeenClosed()
{
	return !Window.isOpen();
}

void PipeMap::Draw()
{
	if (WinSettings.UnitsPerSecond < UnitsSinceLastRender)
	{
		UnitsSinceLastRender -= WinSettings.UnitsPerSecond;

		Window.clear();

		Background.setScale(WinSettings.Info.Scale);
		Window.draw(Background);

		Map.Draw(Window);

		for (auto& Item : Objects)
			Item->Draw(Window);

		WinSettings.Overlay.UpdateAndDraw(Window);

		Window.display();

		++WinSettings.Info.FPS;
	}
}

void PipeMap::MovementTick()
{
	//memset(static_cast<void*>(Map.IsFieldOccupied.data()), 0, sizeof(Map.IsFieldOccupied.data()));

	std::copy(Map.IsFieldOccupiedStaticly.begin(), Map.IsFieldOccupiedStaticly.end(), Map.IsFieldOccupied.begin());

	for (auto& Item : Objects)
	{
		if (!Item->IsStatic())
		{
			Item->Move();

			for (auto Field : Item->OccupiedFields)
			{
				Map.IsFieldOccupied[Field.Point.x][Field.Point.y] = true;
			}		
		}
	}

	Map.MovementTick();

	UnitsSinceLastRender += WinSettings.CurrentTickDuration * WinSettings.CurrentGreatestVolicity;
	
	WinSettings.UpdateGreatestVolicity();
	WinSettings.UpdateTickDuration();

}

void PipeMap::LoadBackground(const std::string & File)
{
	Textures.load(ID::BackGround, File);
}

void PipeMap::LoadObject(const std::string & Path)
{
	std::unique_ptr<IObject> Object = nullptr;

	auto ObjectInfo = stde::LoadDictionary<float>(Path + "/Info.txt");

	if (ObjectInfo.at("IsStatic") != 0.f)
	{
		Object.reset(new StaticObject(WinSettings, Path));

		for (auto Field : Object->OccupiedFields)
		{
			Map.IsFieldOccupied[Field.Point.x][Field.Point.y] = true;
			Map.IsFieldOccupiedStaticly[Field.Point.x][Field.Point.y] = true;			
		}
	}
	else
	{
		throw std::runtime_error("Not implemented yet!");
	}

	Objects.push_back(std::move(Object));
}

void PipeMap::LoadAllObjects(const std::string & Path)
{
	auto ObjectsVector = stde::LoadVector<std::string>(Path + "/Objects.txt");

	for (auto& ObjName : ObjectsVector)
		LoadObject(Path + "/" + ObjName);
}


void PipeMap::SetFlowInitialSpeed(sf::Vector2f Speed)
{
	Map.SetFlowInitialSpeed(Speed);
}

PipeMap::PipeMap(const std::string & Path):
	Background(),
	WinSettings(Path),
	Map(WinSettings),
	CommandsProcessing(WinSettings)
{
	auto texture(new sf::Texture());

	if (!texture->create(WinSettings.Info.MapX, WinSettings.Info.MapY))
		throw std::runtime_error("Texture could not be created!");

	Textures.add(ID::BackGround, texture);
	
	Background.setTexture(*texture, true);

	LoadAllObjects(Path);

	Map.FillUnoccupiedFields();

	Window.create(sf::VideoMode(WinSettings.Info.MapX * WinSettings.Info.Scale.x, WinSettings.Info.MapY * WinSettings.Info.Scale.y), "Fluid Flow");
}


PipeMap::~PipeMap()
{
}

void PipeMap::UpdateMap()
{
}

void PipeMap::ObjectsMovement()
{
}

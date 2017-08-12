#include "PipeMap.h"

std::unique_ptr<PipeMap> PipeMap::GetPipeMapFromUserInput()
{
	while (true)
	{
		try {
			sf::RenderWindow TmpWin;
			Settings LoadingSettings{ TmpWin };

			TextCommands LoadingCommands(LoadingSettings, CommandsSet::MapLoading);

			while (!LoadingSettings.IsMapPathSet())
			{
				LoadingCommands.ProcessCommands();

				if (LoadingSettings.MapState.load() == PipeMapState::Quit)
					return nullptr;

				sf::sleep(sf::milliseconds(1));
			}

			return std::unique_ptr<PipeMap>(new PipeMap(LoadingSettings.GetMapPath()));
		}
		catch (std::exception & E)
		{
			if (E.what())
			{
				sf::RenderWindow TmpWin;
				Settings ErrorSettings{ TmpWin };

				ErrorSettings.Console.Write(std::string(E.what()));

				sf::sleep(sf::seconds(5));
			}
		}
	} 
}

PipeMapState PipeMap::GetMapState()
{
	return WinSettings.MapState.load();
}

void PipeMap::StartSimulation()
{
	while (true)
	{
		if (WinSettings.IsReadyForStart)
		{
			UpdateMap();

			Render = std::move(std::thread{ std::function<void(PipeMap*)> { th }, this });

			WinSettings.WindowEvents.EventSubject.Attach(*this);

			return;
		}
		else
		{
			sf::sleep(sf::milliseconds(10));
			CommandsProcessing.ProcessCommands();
		}
			
	}

}

void PipeMap::SingleTick()
{
	Draw();

	WinSettings.WindowEvents.PoleAllWindowEvents(Window);

	CommandsProcessing.ProcessCommands();
}

bool PipeMap::WindowHasBeenClosed()
{
	return !Window.isOpen();
}

void PipeMap::Draw()
{
	
	//Window.setSize({ WinSettings.Info.MapX*static_cast<unsigned>(WinSettings.Info.Scale.x), WinSettings.Info.MapY*static_cast<unsigned>(WinSettings.Info.Scale.y) });

	if (WinSettings.Info.MapX + 1 < Window.getSize().x / WinSettings.Info.Scale.x || WinSettings.Info.MapX - 1 > Window.getSize().x / WinSettings.Info.Scale.x)
		UpdateMap();


	if (Map.HasBeenRendered.load()){
		std::lock_guard<std::mutex> _lock(SwapMutex);
		FrameBuffer->setActive(true);
		std::swap(LastFrame, FrameBuffer);
		FrameBuffer->setActive(false);
		Map.HasBeenRendered.store(false);
	}

	Window.setActive(true);

	WinSettings.ViewChanger.UpdateView(Window);

	Window.clear();

	Window.draw(sf::Sprite{ LastFrame->getTexture() });

	WinSettings.Overlay.UpdateAndDraw(Window);

	Window.display();

	Window.setActive(false);

	//
	//std::swap(CurrentFrame, NextFrame);
	//auto tmp{ NextFrame.getTexture() };
	//FrameWithoutOverlay.setColor(sf::Color::Green);
	//	CurrentFrame = tmp;

	//Window.setActive(false);
	

	
}

void PipeMap::MovementTick()
{
	//memset(static_cast<void*>(Map.IsFieldOccupied.data()), 0, sizeof(Map.IsFieldOccupied.data()));

	auto Timer{ TimePerTick.MeasureTime() };
	
	std::copy(Map.IsFieldOccupiedStaticly.begin(), Map.IsFieldOccupiedStaticly.end(), Map.IsFieldOccupied.begin());

	WinSettings.Objects->UpdateOccupiedFields(Map.IsFieldOccupied);
	
	Map.MovementTick();

	TimeUnitsSinceLastRender += WinSettings.CurrentTickDuration;
	
	WinSettings.UpdateGreatestVolicity();
	WinSettings.UpdateTickDuration();

	Background.setScale(WinSettings.Info.Scale);

	if (TimeUnitsSinceLastRender >= WinSettings.TimeUnitsPerFrame)
	{
		TimeUnitsSinceLastRender -= WinSettings.TimeUnitsPerFrame;

		std::lock_guard<std::mutex> _lock(SwapMutex);
		FrameBuffer->setActive(true);

		FrameBuffer->draw(Background);

		Map.Draw(*FrameBuffer);

		WinSettings.Objects->Draw(*FrameBuffer);

		FrameBuffer->display();
		FrameBuffer->setActive(false);

		++WinSettings.Info.FPS;
	}
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
		Object.reset(new StaticObject(WinSettings.Info, Path));

		for (auto& Field : Object->OccupiedFields)
			if (Field.Point.x < Map.IsFieldOccupied.size() && Field.Point.y < Map.IsFieldOccupied[Field.Point.x].size())
			{
				Map.IsFieldOccupied[Field.Point.x][Field.Point.y] = true;
				Map.IsFieldOccupiedStaticly[Field.Point.x][Field.Point.y] = true;
			}
	}
	else
	{
		throw std::runtime_error("Not implemented yet!");
	}

	WinSettings.Objects->AddNew(std::move(Object));
}

void PipeMap::LoadAllObjects()
{
	auto ObjectsVector = stde::LoadVector<std::string>(WinSettings.GetMapPath() + "/Objects.txt");

	for (auto& ObjName : ObjectsVector)
		if (ObjName != "")
			LoadObject(WinSettings.GetMapPath() + "/" + ObjName);
}


void PipeMap::SetFlowInitialSpeed(sf::Vector2f Speed)
{
	Map.SetFlowInitialSpeed(Speed);
}

void PipeMap::ExecuteInitScript()
{
	auto Commands = stde::LoadLines(WinSettings.GetMapPath() + "/Init.txt");

	for (auto& Command : Commands)
	{
		CommandsProcessing.ProcessCommand(Command);
	}
}


void th(PipeMap* Map) 
{
	Map->WinSettings.SpawningBoxes->ResetClock();

	while (Map->GetMapState() == PipeMapState::Running)
	{
		Map->MovementTick();
	}
}

PipeMap::PipeMap(const std::string & Path) :
	Background(),
	WinSettings(Path, Window),
	Map(WinSettings),
	CommandsProcessing(WinSettings, CommandsSet::Default),
	FrameBuffer(new sf::RenderTexture),
	LastFrame(new sf::RenderTexture)
{
	auto texture(new sf::Texture());

	if (!texture->create(WinSettings.Info.MapX, WinSettings.Info.MapY))
		throw std::runtime_error("Texture could not be created!");

	Textures.add(ID::BackGround, texture);
	
	Background.setTexture(*texture, true);

	LoadAllObjects();

	std::copy(Map.IsFieldOccupiedStaticly.begin(), Map.IsFieldOccupiedStaticly.end(), Map.IsFieldOccupied.begin());
	WinSettings.Objects->UpdateOccupiedFields(Map.IsFieldOccupied);

	/*Map.UpdatePaths();

	for (auto& Path : NotOccupiedPaths[0])
	{
		Spawner.AddNewSpawningBox(
			SpawningBox(sf::Rect<unsigned>(0, Path.first, 1, Path.second - Path.first - 2), 1.f, FlowInitialSpeed)
		);
	}*/

	//for (unsigned i = 5; i; --i)
	//Map.FillUnoccupiedFields();


	ExecuteInitScript();

	WinSettings.Console << "Map loaded.\n";

	//WinSettings.Overlay.AddItem("TimePerTick", [&]() {return TimePerTick.GetString(); });

}


PipeMap::~PipeMap()
{
	WinSettings.Console << "Freeing resources.\n";
	Render.join();
	Window.setActive(true);
	Window.close();
	Window.setActive(false);
}

void PipeMap::UpdateMap()
{
	std::lock_guard<std::mutex> _lock(SwapMutex);
	Window.setActive(true);

	Window.create(sf::VideoMode(WinSettings.Info.MapX * WinSettings.Info.Scale.x, WinSettings.Info.MapY * WinSettings.Info.Scale.y), "Fluid Flow", sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings::ContextSettings(0,0,0,3,3));

	Window.setFramerateLimit(60);

	Window.setActive(false);
	
	FrameBuffer->setActive(true);

	FrameBuffer->create(WinSettings.Info.MapX * WinSettings.Info.Scale.x, WinSettings.Info.MapY * WinSettings.Info.Scale.y);
	FrameBuffer->setActive(false);
	LastFrame->setActive(true);
	LastFrame->create(WinSettings.Info.MapX * WinSettings.Info.Scale.x, WinSettings.Info.MapY * WinSettings.Info.Scale.y);
	LastFrame->setActive(false);

	Window.setActive(true);
	Window.requestFocus();
	Window.setActive(false);

	WinSettings.ViewChanger.ResetView();
}

void PipeMap::ObjectsMovement()
{
}

void PipeMap::Update(sf::Event & State)
{
	if (State.type == sf::Event::Closed)
	{
		WinSettings.MapState.store(PipeMapState::LoadNewMap);
		return;
	}
}

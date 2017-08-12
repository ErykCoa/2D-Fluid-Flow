#include "WindowViewChanger.h"



void WindowViewChanger::UpdateView(sf::RenderWindow & Window)
{
	Window.setView(CurrentView);
}

void WindowViewChanger::ResetView()
{
	CurrentView.setCenter(Info.MapX * Info.Scale.x / 2.f, Info.MapY * Info.Scale.y / 2.f);
	CurrentView.setSize(Info.MapX * Info.Scale.x, Info.MapY * Info.Scale.y);
}

sf::FloatRect WindowViewChanger::GetViewInfo()
{
	return sf::FloatRect(CurrentView.getCenter() - CurrentView.getSize()/2.f, CurrentView.getSize());
}

WindowViewChanger::WindowViewChanger(WindowInfo & Info, MousePosition & Mouse):
	Info{Info},
	Mouse{Mouse}
{
	ResetView();
}


WindowViewChanger::~WindowViewChanger()
{
}

void WindowViewChanger::Update(sf::Event & State)
{
	if (State.type == sf::Event::MouseWheelScrolled)
	{
		//if (State.mouseWheelScroll.delta > 0)
		{
			CurrentView.zoom(1 - 0.1f * State.mouseWheelScroll.delta);
		}


		if (CurrentView.getSize().x >= Info.MapX * Info.Scale.x)
		{
			ResetView();
		}
		else
		{
			auto NewCenter = (sf::Vector2f(Mouse.GetPosition()) + CurrentView.getCenter()) / 2.f;

			NewCenter.x = stde::Trim(NewCenter.x, CurrentView.getSize().x / 2.f, Info.MapX * Info.Scale.x - CurrentView.getSize().x / 2.f);
			NewCenter.y = stde::Trim(NewCenter.y, CurrentView.getSize().y / 2.f, Info.MapY * Info.Scale.y - CurrentView.getSize().y / 2.f);

			CurrentView.setCenter(NewCenter);
		}
	}
}

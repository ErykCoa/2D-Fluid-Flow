#include "SelectedWindowArea.h"


SelectedWindowArea::SelectedWindowArea(Overlay & DrawOn, WindowInfo & Info, MousePosition & Mouse):
	Info{Info},
	IsFirstCornerSelected{false},
	DrawOn{ DrawOn },
	SpawnBoxOverview{},
	Mouse{Mouse}
{
	SpawnBoxOverview.setFillColor(sf::Color(0,0,0,50));
	SpawnBoxOverview.setOutlineThickness(1.5f);
	SpawnBoxOverview.setOutlineColor(sf::Color(255,255,255,150));
	DrawOn.AddDrawableItem(SpawnBoxOverview);
}

SelectedWindowArea::~SelectedWindowArea()
{
	DrawOn.RemoveDrawableItem(SpawnBoxOverview);
}

void SelectedWindowArea::CaseMouseMoved(sf::Event & State)
{
	CurrentPosition = sf::Vector2u( stde::Trim(Mouse.GetPosition().x, 0, Info.MapX * Info.Scale.x - 1), stde::Trim(Mouse.GetPosition().y, 0, Info.MapY * Info.Scale.y - 1) );

	if (IsFirstCornerSelected && !IsSecondCornerSelected)
	{
		sf::Vector2f Position{};
		sf::Vector2f Size{};

		Position.x = std::min(CurrentPosition.x, FirstCorner.x);
		Position.y = std::min(CurrentPosition.y, FirstCorner.y);

		Size.x = std::abs(static_cast<int>(CurrentPosition.x) - static_cast<int>(FirstCorner.x));
		Size.y = std::abs(static_cast<int>(CurrentPosition.y) - static_cast<int>(FirstCorner.y));
		
		SpawnBoxOverview.setPosition(Position);
		SpawnBoxOverview.setSize(Size);
	}
}

void SelectedWindowArea::CaseMouseButtonPressed(sf::Event & State)
{
	if (State.mouseButton.button == sf::Mouse::Left )
	{
		if (IsFirstCornerSelected && IsSecondCornerSelected)
			Cancel();

		IsFirstCornerSelected = true;
		FirstCorner = CurrentPosition;	
	}
	else if (State.mouseButton.button == sf::Mouse::Right)
	{
		Cancel();
	}

}

void SelectedWindowArea::CaseMouseButtonReleased(sf::Event & State)
{
	if (State.mouseButton.button == sf::Mouse::Left && IsFirstCornerSelected)
	{
		auto CurrentSize = SpawnBoxOverview.getSize();

		if (CurrentSize.x < 5.1f && CurrentSize.y < 5.1f)
		{
			Cancel();
			return;
		}

		IsSecondCornerSelected = true;
		SecondCorner = CurrentPosition;
	}	
}

void SelectedWindowArea::Update(sf::Event & State)
{
	switch (State.type)
	{
	case sf::Event::MouseMoved: CaseMouseMoved(State); break;
	case sf::Event::MouseButtonPressed: CaseMouseButtonPressed(State); break;
	case sf::Event::MouseButtonReleased: CaseMouseButtonReleased(State); break;
	default: break;
	}
}

bool SelectedWindowArea::IsReady()
{
	return IsFirstCornerSelected && IsSecondCornerSelected;
}

sf::Rect<unsigned> SelectedWindowArea::Accept()
{
	if (!IsReady())
		return {0, 0, 0, 0};

	auto Res = sf::Rect<unsigned>(sf::FloatRect{ SpawnBoxOverview.getPosition() / Info.Scale.x, SpawnBoxOverview.getSize() / Info.Scale.x });

	Cancel();

	return Res;
}

void SelectedWindowArea::Cancel()
{
	IsFirstCornerSelected = IsSecondCornerSelected = false;

	SpawnBoxOverview.setSize(sf::Vector2f(0, 0));
}

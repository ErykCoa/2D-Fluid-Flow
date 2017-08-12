#pragma once
#include "IObserver.h"
#include "SFML/Graphics.hpp"
#include "Overlay.h"
#include "stde.h"
#include "WindowInfo.h"
#include "MousePosition.h"

class SelectedWindowArea : public IObserver<sf::Event>
{
public:
	virtual void Update(sf::Event & State) override;

	bool IsReady();

	sf::Rect<unsigned> Accept();
	void Cancel();


	SelectedWindowArea(Overlay & DrawOn, WindowInfo & Info, MousePosition & Mouse);
	~SelectedWindowArea();

private:	
	MousePosition & Mouse;

	void CaseMouseMoved(sf::Event & State);
	void CaseMouseButtonPressed(sf::Event & State);
	void CaseMouseButtonReleased(sf::Event & State);

	WindowInfo & Info;
	Overlay & DrawOn;

	bool IsFirstCornerSelected;
	bool IsSecondCornerSelected;

	sf::Vector2u FirstCorner;
	sf::Vector2u SecondCorner;

	sf::Vector2u CurrentPosition;

	sf::RectangleShape SpawnBoxOverview;
};


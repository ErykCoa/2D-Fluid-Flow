#pragma once
#include "IObserver.h"
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/View.hpp"
#include "stde.h"
#include "WindowInfo.h"
#include "MousePosition.h"

class WindowViewChanger : public IObserver<sf::Event>
{
public:
	void UpdateView(sf::RenderWindow & Window);
	void ResetView();

	sf::FloatRect GetViewInfo();

	WindowViewChanger(WindowInfo & Info, MousePosition & Mouse);
	~WindowViewChanger();

	virtual void Update(sf::Event & State) override;

private:
	WindowInfo & Info;
	MousePosition & Mouse;

	sf::View CurrentView;
};


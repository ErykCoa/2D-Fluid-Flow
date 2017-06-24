#pragma once
#include "SFML/Graphics.hpp"
#include <map>
#include <functional>
#include <string>
#include "WindowInfo.h"
#include "stde.h"
#include "Consts.h"

class Overlay
{
public:
	void UpdateAndDraw(sf::RenderWindow & Window);

	void AddItem(std::string Name, std::function<std::string()> Func);
	void RemoveItem(std::string Name);

	void SetVisability(bool Visible);
	bool GetVisability();
	void SetFontSize(unsigned int Size);
	void SetFontStyle(sf::Text::Style Style);

	bool HasItem(std::string Key);

	void ChangeFontStyle(sf::Text::Style Style);

	Overlay(WindowInfo & Info);
private:
	void Draw(sf::RenderWindow & Window);
	void Update();

	WindowInfo & Info;

	bool Visible;

	std::map<std::string, std::function<std::string()>> UIElements;

	sf::Text Text;
	sf::Font Font;
};


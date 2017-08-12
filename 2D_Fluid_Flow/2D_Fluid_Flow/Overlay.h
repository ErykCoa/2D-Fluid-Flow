#pragma once
#include "SFML/Graphics.hpp"
#include <map>
#include <functional>
#include <string>
#include "WindowInfo.h"
#include "stde.h"
#include "Consts.h"
#include <functional>
#include "WindowViewChanger.h"

class Overlay
{
public:
	void UpdateAndDraw(sf::RenderTarget & Window);

	void AddItem(std::string Name, std::function<std::string()> Func);
	void RemoveItem(std::string Name);

	void AddDrawableItem(sf::Drawable & Item);
	void RemoveDrawableItem(sf::Drawable & Item);

	void SetVisability(bool Visible);
	bool GetVisability();
	void SetFontSize(unsigned int Size);
	void SetFontStyle(sf::Text::Style Style);

	bool HasItem(std::string Key);

	void ChangeFontStyle(sf::Text::Style Style);

	Overlay(WindowInfo & Info, WindowViewChanger & View);
private:
	WindowViewChanger & View;
	void Draw(sf::RenderTarget  & Window);
	void Update();

	WindowInfo & Info;

	bool Visible;


	std::map<std::string, std::function<std::string()>> UIElements;

	using DrawableRef = typename std::reference_wrapper<sf::Drawable>;
	std::set<DrawableRef, stde::CompereAddresses<DrawableRef>> AdditionalDrawableItems;

	sf::Text Text;
	sf::Font Font;
};


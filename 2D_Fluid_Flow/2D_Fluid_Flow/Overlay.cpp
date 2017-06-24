#include "Overlay.h"

void Overlay::UpdateAndDraw(sf::RenderWindow & Window)
{
	Update();
	Draw(Window);
}

void Overlay::Draw(sf::RenderWindow & Window)
{
	Text.setScale(Info.Scale);
	Window.draw(Text);
}

void Overlay::Update()
{
	std::string Content;
	std::string Tmp;

	unsigned CharactersCount{};

	for (auto& Item : UIElements)
	{
		Tmp = Item.first + " : " + Item.second() + "  ";
		CharactersCount += Tmp.size();

		if (CharactersCount*Text.getCharacterSize()*FontWidthToSizePerChar > Info.MapX && Content.size())
		{
			CharactersCount = Tmp.size();
			Content += '\n';
		}

		Content += Tmp;
	}

	Text.setString(Content);
}

void Overlay::AddItem(std::string Name, std::function<std::string()> Func)
{
	UIElements[Name] = Func;
}

void Overlay::RemoveItem(std::string Name)
{
	UIElements.erase(Name);
}

void Overlay::SetVisability(bool Visible)
{
	this->Visible = Visible;
}

bool Overlay::GetVisability()
{
	return Visible;
}

void Overlay::SetFontSize(unsigned int Size)
{
	Text.setCharacterSize(Size);
}

void Overlay::SetFontStyle(sf::Text::Style Style)
{
	Text.setStyle(Style);
}

bool Overlay::HasItem(std::string Key)
{
	return UIElements.find(Key) != UIElements.end();
}

void Overlay::ChangeFontStyle(sf::Text::Style Style)
{
	Text.setStyle(Text.getStyle() ^ Style);
}

Overlay::Overlay(WindowInfo & Info) :
Info(Info),
Visible(true)
{
	if (!Font.loadFromFile("Fonts/LiberationMono-Bold.ttf"))
		throw std::runtime_error("Failed to open file: Fonts/LiberationMono-Bold.ttf");

	Text.setFont(Font);
	Text.setCharacterSize(stde::Trim(Info.MapY / 30, 12, 20));

	Text.setColor(sf::Color::White);
}


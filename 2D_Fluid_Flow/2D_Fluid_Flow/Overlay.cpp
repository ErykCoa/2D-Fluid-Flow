#include "Overlay.h"

void Overlay::UpdateAndDraw(sf::RenderTarget & Window)
{
	Update();
	Draw(Window);
}

void Overlay::Draw(sf::RenderTarget & Window)
{
	//Text.setScale(Info.Scale);
	for (auto & Item : AdditionalDrawableItems)
		Window.draw(Item);

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

	sf::FloatRect ViewInfo = View.GetViewInfo();

	Text.setScale( sf::Vector2f{1.f,1.f} * static_cast<float>(ViewInfo.width / (Info.MapX * Info.Scale.x )));
	Text.setPosition(ViewInfo.left, ViewInfo.top);
}

void Overlay::AddItem(std::string Name, std::function<std::string()> Func)
{
	UIElements[Name] = Func;
}

void Overlay::RemoveItem(std::string Name)
{
	UIElements.erase(Name);
}

void Overlay::AddDrawableItem(sf::Drawable & Item)
{
	AdditionalDrawableItems.insert(Item);
}

void Overlay::RemoveDrawableItem(sf::Drawable & Item)
{
	if (AdditionalDrawableItems.find(Item) == AdditionalDrawableItems.end())
		throw std::logic_error("AdditionalDrawableItems does not contain the Item!");

	AdditionalDrawableItems.erase(Item);
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

Overlay::Overlay(WindowInfo & Info, WindowViewChanger & View) :
Info(Info),
Visible(true),
View{View}
{
	if (!Font.loadFromFile("Fonts/LiberationMono-Bold.ttf"))
		throw std::runtime_error("Failed to open file: Fonts/LiberationMono-Bold.ttf");

	Text.setFont(Font);
	Text.setCharacterSize(stde::Trim(Info.MapY / 30, 12, 20));

	Text.setColor(sf::Color::White);
}


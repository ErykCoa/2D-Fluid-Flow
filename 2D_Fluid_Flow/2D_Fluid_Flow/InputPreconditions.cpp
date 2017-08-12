#include "InputPreconditions.h"

bool InputPreconditions::IsOnScreen(Settings & WinSettings, int RelativeX, int RelativeY)
{
	return (RelativeX < 0 || RelativeX > WinSettings.Info.MapX) ? false : (RelativeY < 0 || RelativeY > WinSettings.Info.MapY) ? false : true;
}

bool InputPreconditions::IsOnScreen(Settings & WinSettings, sf::Event::MouseButtonEvent & EventInfo)
{
	return (EventInfo.x < 0 || EventInfo.x > WinSettings.Info.MapX) ? false : (EventInfo.y < 0 || EventInfo.y > WinSettings.Info.MapY) ? false : true;
}

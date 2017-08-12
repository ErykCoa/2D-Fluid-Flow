#pragma once
#include "SFML/Graphics.hpp"
#include <string>
#include <functional>

namespace {
	class TimeCounterHelper {
	public:
		TimeCounterHelper(std::function<void()> AtFinish) :
			AtFinish(AtFinish)
		{}

		~TimeCounterHelper()
		{
			AtFinish();
		}

	private:
		std::function<void()> AtFinish;
	};
}
class TimeCounter
{
public:
	TimeCounter();

	std::string GetString();

	TimeCounterHelper MeasureTime()
	{
		Clock.restart();

		return TimeCounterHelper([&]() { ElapsedTime = Clock.getElapsedTime().asMicroseconds() / 1000.; });
	}

private:
	sf::Clock Clock;
	sf::Time TimeSinceLastUpdate;
	double ElapsedTime;
};



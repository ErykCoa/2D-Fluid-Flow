#include "TimeCounter.h"

TimeCounter::TimeCounter() :
	TimeSinceLastUpdate{ sf::Time::Zero },
	ElapsedTime{ 0 }
{
}

std::string TimeCounter::GetString()
{
	return std::to_string(ElapsedTime) + " ms";
}

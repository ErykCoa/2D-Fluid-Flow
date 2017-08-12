#pragma once

#include "SFML/Window/Event.hpp"
#include <queue>
#include <mutex>
#include <functional>
#include <exception>
#include "IObserver.h"

template<sf::Event::EventType EventType>
class InteractiveWindowInputEvents : public IObserver<sf::Event>
{
public:
	virtual void Update(sf::Event & State) override;

	bool IsInputQueueEmpty();

	sf::Event PopInput();

	bool IsAwaitingInput();

	void StartAwaitingInput(std::function<bool(sf::Event&)> IsInputOK);

	void StopAwaitingInput();

	InteractiveWindowInputEvents();
private:
	bool AwaitsInput;

	std::function<bool(sf::Event&)> IsInputOK;

	std::mutex InputMutex;

	std::queue<sf::Event> InputEvents;
};


template<sf::Event::EventType EventType>
inline void InteractiveWindowInputEvents<EventType>::Update(sf::Event & State)
{
	if (State.type == EventType)
	{
		std::lock_guard<std::mutex> _lock(InputMutex);
		InputEvents.push(State);
	}	
}

template<sf::Event::EventType EventType>
inline bool InteractiveWindowInputEvents<EventType>::IsInputQueueEmpty()
{
	std::lock_guard<std::mutex> _lock(InputMutex);
	return InputEvents.empty();
}

template<sf::Event::EventType EventType>
inline sf::Event InteractiveWindowInputEvents<EventType>::PopInput()
{
	std::lock_guard<std::mutex> _lock(InputMutex);

	auto res = InputEvents.front();

	InputEvents.pop();

	return std::move(res);
}

template<sf::Event::EventType EventType>
inline bool InteractiveWindowInputEvents<EventType>::IsAwaitingInput()
{
	std::lock_guard<std::mutex> _lock(InputMutex);
	return AwaitsInput;
}

template<sf::Event::EventType EventType>
inline void InteractiveWindowInputEvents<EventType>::StartAwaitingInput(std::function<bool(sf::Event&)> IsInputOK)
{
	if (AwaitsInput)
		throw std::logic_error("Input is already awaited!");

	std::lock_guard<std::mutex> _lock(InputMutex);

	while (!InputEvents.empty())
		InputEvents.pop();

	this->IsInputOK = IsInputOK;

	AwaitsInput = true;
}

template<sf::Event::EventType EventType>
inline void InteractiveWindowInputEvents<EventType>::StopAwaitingInput()
{
	std::lock_guard<std::mutex> _lock(InputMutex);

	AwaitsInput = false;
}

template<sf::Event::EventType EventType>
inline InteractiveWindowInputEvents<EventType>::InteractiveWindowInputEvents() :
	AwaitsInput{ false }
{
}



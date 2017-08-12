#pragma once

#include "IObserver.h"
#include <set>
#include "stde.h"

template<class StateObjectType>
class Subject{
public:
	StateObjectType & GetState() 
	{ 
		return State 
	};

	void SetState(StateObjectType & State) 
	{ 
		this->State = State;
		NotifyAllObservers();
	};

	void Attach(IObserver<StateObjectType> & Observer)
	{
		Observers.insert(Observer);
	}

	void Detach(IObserver<StateObjectType> & Observer)
	{
		Observers.erase(Observer);
	}

	void NotifyAllObservers() 
	{
		for (IObserver<StateObjectType> & Observer : Observers)
			Observer.Update(State);
	}

private:
	StateObjectType State;

	using IObserverRef = typename std::reference_wrapper<IObserver<StateObjectType>>;
	std::set<IObserverRef, stde::CompereAddresses<IObserverRef>> Observers;
};

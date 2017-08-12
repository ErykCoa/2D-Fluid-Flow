#pragma once

template<class StateObjectType>
class IObserver {
public:
	virtual void Update(StateObjectType & State) = 0;
};
#pragma once

class IFluidVelocityMap {
public:
	virtual void FillUnoccupiedFields() = 0;
};
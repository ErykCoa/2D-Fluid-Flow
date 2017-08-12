#pragma once
#include <vector>
#include "SFML/Graphics.hpp"
#include <memory>
#include "StructsAndEnums.h"
#include "IObject.h"
#include "Subject.h"

class IObjectsHandler : public Subject<IObject*> 
{
public:
	virtual void UpdateOccupiedFields(std::vector<std::vector<bool>> & In) = 0;
	virtual void AddNew(std::unique_ptr<IObject> Object) = 0;
	virtual void SetVisability(VisabilityType To) = 0;
	virtual void Move() = 0;
	virtual void Draw(sf::RenderTarget & Window) = 0;
	virtual void AwaitSelection() = 0;
	virtual bool IsAnySelected() = 0;
	virtual void DeleteSelected() = 0;
	virtual void CancelSelection() = 0;

	virtual ~IObjectsHandler() {};
	std::vector<std::unique_ptr<IObject>> Objects;


protected:


};
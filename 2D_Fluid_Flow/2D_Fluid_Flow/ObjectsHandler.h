#pragma once
#include "IObjectsHandler.h"
#include "IObserver.h"
#include "SFML/Window/Event.hpp"
#include <mutex>
#include "MousePosition.h"

class ObjectsHandler : public IObjectsHandler, public IObserver<sf::Event>
{
public:
	virtual void AddNew(std::unique_ptr<IObject> Object) override;
	virtual void SetVisability(VisabilityType To) override;
	virtual void Move() override;
	virtual void Draw(sf::RenderTarget & Window) override;
	virtual bool IsAnySelected() override;
	virtual void DeleteSelected() override;
	virtual void AwaitSelection() override;

	virtual void CancelSelection() override;

	virtual void UpdateOccupiedFields(std::vector<std::vector<bool>>& In) override;

	virtual void Update(sf::Event & State) override;

	ObjectsHandler(MousePosition & Mouse);
	~ObjectsHandler();

private:
	std::mutex ObjectsMutex;
	MousePosition & Mouse;

	bool AwaitsSelection;
	bool IsSelected;
	decltype(Objects)::iterator SelectedIndex;


};


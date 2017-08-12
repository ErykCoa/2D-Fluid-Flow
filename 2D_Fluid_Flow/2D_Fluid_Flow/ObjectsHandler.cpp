#include "ObjectsHandler.h"


ObjectsHandler::ObjectsHandler(MousePosition & Mouse) :
	IsSelected{false},
	AwaitsSelection{false},
	Mouse{Mouse}
{
}

ObjectsHandler::~ObjectsHandler()
{
}

void ObjectsHandler::UpdateOccupiedFields(std::vector<std::vector<bool>>& In)
{
	std::lock_guard<std::mutex> _lock(ObjectsMutex);
	for (auto& Item : Objects)
	{
		if (!Item->IsStatic())
		{
			Item->Move();

			for (auto& Field : Item->OccupiedFields)
				for (auto& Field : Item->OccupiedFields)
				{
					In[Field.Point.x][Field.Point.y] = true;
				}
		}
	}
}

void ObjectsHandler::CancelSelection()
{
	std::lock_guard<std::mutex> _lock(ObjectsMutex);
	if (IsSelected)
	{
		SelectedIndex->get()->Visability(VisabilityType::Default);
		IsSelected = false;
	}
	AwaitsSelection = false;
}

void ObjectsHandler::Update(sf::Event & State)
{
	if (State.type == sf::Event::MouseButtonPressed && State.mouseButton.button == sf::Mouse::Button::Left && AwaitsSelection)
	{
		std::lock_guard<std::mutex> _lock(ObjectsMutex);
		for (auto & Object = Objects.rbegin(); Object != Objects.rend(); ++Object)
			if (Object->get()->IsAt(sf::Vector2u( Mouse.GetPosition().x, Mouse.GetPosition().y)))
			{
				for (auto& Object : Objects)
					Object->Visability(VisabilityType::Default);

				SelectedIndex = (Object + 1).base();
				SelectedIndex->get()->Visability(VisabilityType::Selected);
				IsSelected = true;
				AwaitsSelection = false;
				return;
			}
	}
	else if (State.type == sf::Event::MouseButtonPressed && State.mouseButton.button == sf::Mouse::Button::Right)
	{
		CancelSelection();
	}
}

void ObjectsHandler::AwaitSelection()
{
	AwaitsSelection = true;
}

void ObjectsHandler::AddNew(std::unique_ptr<IObject> Object)
{
	std::lock_guard<std::mutex> _lock(ObjectsMutex);
	Objects.push_back(std::move(Object));
}

void ObjectsHandler::SetVisability(VisabilityType To)
{
	std::lock_guard<std::mutex> _lock(ObjectsMutex);
	for (auto& Object : Objects)
		Object->Visability(To);
}

void ObjectsHandler::Move()
{
	std::lock_guard<std::mutex> _lock(ObjectsMutex);
	for (auto& Object : Objects)
		if (!Object->IsStatic())
			Object->Move();
}

void ObjectsHandler::Draw(sf::RenderTarget & Window)
{
	std::lock_guard<std::mutex> _lock(ObjectsMutex);
	for (auto& Object : Objects)
		Object->Draw(Window);
}

bool ObjectsHandler::IsAnySelected()
{
	return IsSelected;
}

void ObjectsHandler::DeleteSelected()
{
	if (IsSelected)
	{
		std::lock_guard<std::mutex> _lock(ObjectsMutex);
		SetState(SelectedIndex->_Myptr());
		Objects.erase(SelectedIndex);
		IsSelected = false;
	}	
}

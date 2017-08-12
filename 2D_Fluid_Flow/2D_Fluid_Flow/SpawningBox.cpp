#include "SpawningBox.h"


SpawningBox::SpawningBox(sf::Rect<unsigned> Field, float Thickness, sf::Vector2f & StartingVolacity) :
	Field(Field),
	Thickness(Thickness),
	StartingVolacity(StartingVolacity),
	PositionXDis(Field.left + Field.width / 2.f, Field.width / 5.f),
	PositionYDis(Field.top + Field.height / 2.f, Field.height / 5.f),
	VolacityXDis(StartingVolacity.x, fabs(StartingVolacity.x) * 0.1f + 0.01f),
	VolacityYDis(StartingVolacity.y, fabs(StartingVolacity.y) * 0.1f + 0.01f)
{
}



#pragma once

#include "SFML/Graphics.hpp"
#include <functional>
#include <map>
#include <string>
#include <random>


enum class ID {
	BackGround
};

enum class RenderType {
	None,
	Regular,
	Circles
};

struct OccupiedField {
	sf::Vector2u Point;
	sf::Uint8 Value;
};

struct PairsOnceChecked {
	unsigned first;
	unsigned second;
	unsigned x;
};

template<typename T>
struct Boundaries
{
	T LowerX;
	T UpperX;
	T LowerY;
	T UpperY;
};

struct CommandInfo {
	std::function<void(std::string)> Function;
	std::map<char, std::function<void()>> AdditionalParametrs;
	std::string Help;
	std::string MoreHelp;
};

struct SpawningBox {
	sf::Rect<unsigned> Field;

	float Thickness;
	sf::Vector2f StartingVolacity;
	std::normal_distribution<float> PositionXDis;
	std::normal_distribution<float> PositionYDis;
	std::normal_distribution<float> VolacityXDis;
	std::normal_distribution<float> VolacityYDis;


	SpawningBox(sf::Rect<unsigned> Field, float Thickness, sf::Vector2f & StartingVolacity):
		Field(Field),
		Thickness(Thickness),
		StartingVolacity(StartingVolacity),
		PositionXDis(Field.left + Field.width / 2.f, Field.width / 5.f),
		PositionYDis(Field.top + Field.height / 2.f, Field.height / 5.f),
		VolacityXDis(StartingVolacity.x, fabs(StartingVolacity.x) * 0.1f + 0.01f),
		VolacityYDis(StartingVolacity.y, fabs(StartingVolacity.y) * 0.1f + 0.01f)
	{
	}
};



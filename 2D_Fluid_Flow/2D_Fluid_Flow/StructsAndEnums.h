#pragma once

#include "SFML/Graphics.hpp"
#include <functional>
#include <map>
#include <string>
#include <random>
#include <set>
#include "CommandID.h"


enum class ID {
	BackGround
};

enum class RenderType {
	None,
	Regular,
	Circles
};

enum class CommandsSet {
	MapLoading,
	Default
};

enum class CommandStatus {
	Completed,
	Pending,
	Failed
};

enum class VisabilityType {
	Default,
	Hidden,
	Visible,
	Selected
};

enum class BorderType {
	Normal,
	Bouncy
};

enum class PipeMapState {
	Running,
	Quit,
	LoadNewMap
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
	CommandID ID;
	std::function<CommandStatus(std::string, std::set<char> )> Function;
	std::string Help;
	std::string MoreHelp;

	bool operator<(const CommandInfo & With) const
	{
		return ID < With.ID;
	}
};


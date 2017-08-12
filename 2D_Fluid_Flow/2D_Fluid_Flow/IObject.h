#pragma once
#include <vector>
#include "SFML/Graphics.hpp"
#include <memory>
#include <fstream>
#include "StructsAndEnums.h"
#include "stde.h"
#include "Consts.h"
#include <mutex>
#include "WindowInfo.h"

class IObject
{
public:
	std::vector<OccupiedField> OccupiedFields;

	virtual bool IsStatic() = 0;
	virtual void Move() = 0;
	virtual void Draw(sf::RenderTarget & Window) = 0;
	virtual void DeleteAllReferences() {}
	virtual void Visability(VisabilityType SetTo);
	virtual bool IsAt(sf::Vector2u Coordinates);
	virtual ~IObject() {};

	IObject(WindowInfo & Info) : Info{ Info }, Texture(new sf::Texture), CurrentVisability(VisabilityType::Default), OccupiedFields(Info.MapX){}

protected:
	WindowInfo & Info;

	sf::Vector2f AbsolutePosition;
	VisabilityType CurrentVisability;

	sf::Sprite Sprite;
	std::unique_ptr<sf::Texture> Texture;
	sf::Image Image;
	sf::Image HitBox;

	virtual void UpdateHitBox();
	virtual void LoadHitBox(const std::string & Path);
	virtual void LoadTexture(const std::string & Path);
	virtual void LoadSpritePosition(const std::string & Path);
};

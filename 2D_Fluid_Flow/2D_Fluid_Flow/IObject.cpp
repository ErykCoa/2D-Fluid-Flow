#include "IObject.h"

void IObject::UpdateHitBox()
{
	const auto HitBoxX = HitBox.getSize().x;
	const auto HitBoxY = HitBox.getSize().y;
	const auto Size = HitBoxX * HitBoxY;

	const auto Begin = (sf::Uint32*)(HitBox.getPixelsPtr());

	std::mutex Lock;
	sf::Vector2u OffSet(Sprite.getPosition().x, Sprite.getPosition().y);
	
	stde::For_each<NumberOfThreadsRunning>( Begin, Begin + Size, 
		[&](decltype((sf::Uint32*)(HitBox.getPixelsPtr())) Iter)
	{
		auto Number = std::distance(Begin, Iter);

		sf::Vector2u Index{ Number % HitBoxX , Number / HitBoxX };

		auto Pixel = (sf::Uint8*)Iter;

		if (Pixel[3] != 0)
		{
			std::lock_guard<std::mutex> _lock(Lock);
			OccupiedFields.push_back({ Index + OffSet, Pixel[3] });
		}
			
	});

}

void IObject::LoadHitBox(const std::string & Path)
{
	if (!HitBox.loadFromFile(Path + "/HitBox.png"))
		throw std::runtime_error("Failed to load hitbox from file: " + Path + "/HitBox.png");
}

void IObject::LoadTexture(const std::string & Path)
{
	if (!Image.loadFromFile(Path + "/Texture.png"))
		throw std::runtime_error("Failed to load texture from file: " + Path + "/Texture.png");

	if (!Texture->loadFromImage(Image))
		throw std::runtime_error("Failed to load texture from Image!");

	Sprite.setTexture(*Texture, true);
}

void IObject::LoadSpritePosition(const std::string & Path)
{
	auto Info = stde::LoadDictionary<float>(Path + "/Info.txt");

	Sprite.setPosition(stde::GetValue(Info, "PositionX"), stde::GetValue(Info, "PositionY"));

}

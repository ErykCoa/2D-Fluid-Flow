#include "Particle.h"

std::default_random_engine Particle::Engine{};

void Particle::Collide(Particle & Particle)
{
	const auto Distance = sqrt(pow(Particle.Position.x - Position.x, 2) + pow(Particle.Position.y - Position.y, 2));

	if (2 * WinSettings.ParticleRadius > Distance)
	{		
		SpringlikeColision(Particle.Position);
	}
}

void Particle::Collide(std::vector<std::vector<bool>> & IsFieldOccupied)
{
	sf::Vector2f ClosestPoint{0,0};
	float ShortestDistance{FP_INFINITE};

	

	for (int X = stde::Trim(BoundryX.x, 0, WinSettings.Info.MapX); X < BoundryX.y && X < WinSettings.Info.MapX; ++X)
		for (int Y = stde::Trim(BoundryY.x, 0, WinSettings.Info.MapY); Y < BoundryY.y && Y < WinSettings.Info.MapY; ++Y)
			if (IsFieldOccupied[X][Y])
			{
				const auto Distance = sqrt(pow(X + 0.5f - Position.x, 2) + pow(Y + 0.5f - Position.y, 2));
				//const auto Distance2 = sqrt(pow(X + 0.5f - Position.x - Velocity.x * WinSettings.CurrentTickDuration, 2) + pow(Y + 0.5f - Position.y - Velocity.y* WinSettings.CurrentTickDuration, 2));

				if (Distance < WinSettings.ParticleRadius + 0.5f && Distance < ShortestDistance)
				{
					ShortestDistance = Distance;

					//SolidColision(sf::Vector2f{X + 0.5f, Y + 0.5f});

					ClosestPoint = sf::Vector2f{ X + 0.5f, Y + 0.5f };
				}
			}

	if (ShortestDistance != FP_INFINITE)
	{
		
		SolidColision(ClosestPoint);
		NextPosition += (NextPosition - ClosestPoint) * (2 * WinSettings.ParticleRadius - sqrt(pow(ClosestPoint.x - Position.x, 2) + pow(ClosestPoint.y - Position.y, 2))) / (WinSettings.ParticleRadius + 0.5f);
	}
}

void Particle::CollideWithBorders()
{
	if (Position.x - WinSettings.ParticleRadius < 0)
	{
		if (NextVelocity.x < 0)
			NextVelocity.x *= -1;
	}
	else if (Position.x + WinSettings.ParticleRadius > WinSettings.Info.MapX)
	{

	}

	if (Position.y - WinSettings.ParticleRadius < 0)
	{

	}
	else if (Position.y + WinSettings.ParticleRadius > WinSettings.Info.MapY)
	{

	}
}

void Particle::Move()
{
	NextPosition += NextVelocity * WinSettings.CurrentTickDuration;


	if (Position.x > static_cast<int>(Position.x) + 0.5f)
	{
		BoundryX = { Position.x, Position.x + WinSettings.ParticleRadius };
	}
	else
	{
		BoundryX = { Position.x - WinSettings.ParticleRadius, Position.x };
	}

	if (Position.y > static_cast<int>(Position.y) + 0.5f)
	{
		BoundryY = { Position.y, Position.y + WinSettings.ParticleRadius };
	}
	else
	{
		BoundryY = { Position.y - WinSettings.ParticleRadius, Position.y };
	}

}

void Particle::Draw(sf::RenderWindow & Window)
{
	UpdateCircle(WinSettings.Info.Scale, WinSettings.Contrast);

	Window.draw(Circle);
}

bool Particle::HasLeftMap()
{
	return Position.x < 0 || Position.y < 0 || Position.x > WinSettings.Info.MapX || Position.y > WinSettings.Info.MapY;
}


void Particle::Update()
{
	Position = NextPosition;
	Velocity = NextVelocity;

	WinSettings.TrySetNextGreatestVolicity(sfe::Length(Velocity));
}

Particle::Particle(Settings & WinSettings, sf::Vector2f StartingPosition, sf::Vector2f StartingVelocity) :
	Velocity(StartingVelocity),
	NextVelocity(StartingVelocity),
	Position(StartingPosition),
	NextPosition(StartingPosition),
	WinSettings(WinSettings)
{
	Circle = sf::CircleShape(WinSettings.ParticleRadius);
}

Particle::Particle(Settings & WinSettings, 
	sf::Vector2f StartingPosition, Boundaries<float>& PositionBoun, std::normal_distribution<float>& PositionDis, 
	sf::Vector2f StartingVolacity, Boundaries<float>& VolacityBoun, std::normal_distribution<float>& VolacityDis) :
	WinSettings{ WinSettings }
{
	++WinSettings.ParticlesCounter;

	 Position = NextPosition = stde::Trim(sf::Vector2f{ StartingPosition.x + PositionDis(Engine), StartingPosition.y + PositionDis(Engine) }, PositionBoun);
	 Velocity = NextVelocity = stde::Trim(sf::Vector2f{ StartingVolacity.x + VolacityDis(Engine), StartingVolacity.y + VolacityDis(Engine) }, VolacityBoun);

	Circle = sf::CircleShape(WinSettings.ParticleRadius);
}


Particle::~Particle()
{
}

void Particle::UpdateCircle(sf::Vector2f Scale, float Contrast)
{
	unsigned tmp = (fabs(Velocity.x) + fabs(Velocity.y))*Contrast;
	tmp = tmp >= 255 ? 255 : tmp;

	Circle.setFillColor(sf::Color(
		tmp,
		0,
		255 - tmp
		)
	);

	Circle.setPosition((Position - sf::Vector2f(WinSettings.ParticleRadius, WinSettings.ParticleRadius)) * Scale.x);

	Circle.setScale(Scale);

}

inline void Particle::SolidColision(sf::Vector2f & With)
{


	sf::Transform rotation{};

	auto res2 = -90 + ((atan2(Position.y - With.y, Position.x - With.x) - atan2(Position.y - Velocity.y, Position.x - Velocity.x)) * 180 / PI);
		
	

	rotation.rotate( res2);

	auto res = rotation.transformPoint(rotation.transformPoint(NextVelocity));

	NextVelocity = res;



	//TODO mirrorlike collision
}

inline void Particle::SpringlikeColision(sf::Vector2f & With)
{
	NextVelocity += sf::Vector2f{
		(fabs(Position.x - With.x) < 0.001f) ? 0 : 1 / (Position.x - With.x),
		(fabs(Position.y - With.y) < 0.001f) ? 0 : 1 / (Position.y - With.y)
	} * WinSettings.BaseSpringinessForce;
}

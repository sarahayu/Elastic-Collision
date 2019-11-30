#pragma once
#include <SFML\Graphics.hpp>

namespace Settings
{
	const float TIME_TO_COLLISION = 2.f;
	const float COASTING_TIME = 0.2f;
	const float ACCEL_TIME = TIME_TO_COLLISION - COASTING_TIME;

	sf::Font& getArialFont();
}
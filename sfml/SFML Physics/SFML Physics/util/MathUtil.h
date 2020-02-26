#pragma once
#include <SFML\Graphics.hpp>

namespace mu
{
	const float PI = 3.141599265f;
	const float TWO_PI = PI * 2;

	const sf::Vector2f polarToRectangular(const sf::Vector2f &polarCoordinates);
	const sf::Vector2f polarToRectangular(const float &radius, const float &theta);
	const sf::Vector2f rectangularToPolar(const sf::Vector2f &rectCoordinates);
	const sf::Vector2f rectangularToPolar(const float &x, const float &y);

	const sf::Vector2f rotateVector(const sf::Vector2f &centered, const float &rotateBy, const bool &polar = false);

	const float radToDeg(const float &radians);
	const float degToRad(const float &degrees);
}
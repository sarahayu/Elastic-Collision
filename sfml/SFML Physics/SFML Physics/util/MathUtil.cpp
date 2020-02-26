#include "MathUtil.h"
#include <cmath>

const sf::Vector2f mu::polarToRectangular(const sf::Vector2f & polarCoordinates)
{
	return polarToRectangular(polarCoordinates.x, polarCoordinates.y);
}

const sf::Vector2f mu::polarToRectangular(const float & radius, const float & theta)
{
	return{ radius * std::cos(degToRad(theta)), radius * std::sin(degToRad(theta)) };
}

const sf::Vector2f mu::rectangularToPolar(const sf::Vector2f & rectCoordinates)
{
	return rectangularToPolar(rectCoordinates.x, rectCoordinates.y);
}

const sf::Vector2f mu::rectangularToPolar(const float & x, const float & y)
{
	return{ std::sqrt(x*x + y*y), radToDeg(std::atan2f(y, x)) };
}

const sf::Vector2f mu::rotateVector(const sf::Vector2f & centered, const float & rotateBy, const bool & polar)
{
	sf::Vector2f toRotate = centered;
	if (!polar) toRotate = rectangularToPolar(centered);

	toRotate.y += rotateBy;

	return toRotate;
}

const float mu::radToDeg(const float & radians)
{
	return radians * 180.f / PI;
}

const float mu::degToRad(const float & degrees)
{
	return degrees * PI / 180.f;
}
#include "Ball.h"
#include "Settings.h"
#include <sstream>
#include <iomanip>

namespace
{
	const std::string setPrecision(float f)
	{
		std::stringstream ss;
		ss << std::setprecision(2) << f;
		return ss.str();
	}
}

Ball::Ball(const sf::Color &color)
{
	circle.setFillColor(color);

	massText.setFont(Settings::getArialFont());
	massText.setString(setPrecision(mass));
	massText.setOutlineThickness(1.f);
	massText.setOutlineColor(sf::Color(100, 100, 100));
}

void Ball::initPhysics(const sf::Vector2f &position, const sf::Vector2f &acceleration, const float &mass)
{
	this->velocity = sf::Vector2f();
	this->acceleration = acceleration;
	this->mass = mass;

	const float radius = getRadius(mass);
	circle.setRadius(radius);
	circle.setOrigin(radius, radius);

	circle.setPosition(position);

	massText.setString(setPrecision(mass));
	sf::FloatRect bounds = massText.getLocalBounds();
	massText.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
}

const bool Ball::resolveCollision(Ball & other)
{
	const float minDistance = other.getRadius() + getRadius();
	const sf::Vector2f positionDifference = other.circle.getPosition() - circle.getPosition();
	const sf::Vector2f absPosDifference(std::abs(positionDifference.x), std::abs(positionDifference.y));
	const float distSquared = absPosDifference.x * absPosDifference.x + absPosDifference.y * absPosDifference.y;

	if (distSquared >= minDistance * minDistance)
		return false;

	const float distance = std::sqrt(distSquared);
	const float halfIntersection = (minDistance - distance) / 2;
	const float pushX = halfIntersection * absPosDifference.x / distance;
	const float pushY = halfIntersection * absPosDifference.y / distance;

	Ball *first = this, *second = &other;

	if (first->circle.getPosition().x > second->circle.getPosition().x) std::swap(first, second);
	first->circle.move(-pushX, 0.f);
	second->circle.move(pushX, 0.f);

	if (first->circle.getPosition().y > second->circle.getPosition().y) std::swap(first, second);
	first->circle.move(0.f, -pushY);
	second->circle.move(0.f, pushY);

	return true;
}

float Ball::getRadius(const float & mass)
{
	return std::min(100.f, 50 + mass * 50 / 10);
}

const float Ball::getRadius() const
{
	return circle.getRadius();
}

void Ball::update(const float &deltatime)
{
	velocity += acceleration * deltatime;
	circle.move(velocity * deltatime);
}

void Ball::draw(sf::RenderWindow & window)
{
	massText.setPosition(circle.getPosition());
	window.draw(circle);
	window.draw(massText);
}
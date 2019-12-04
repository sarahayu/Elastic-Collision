#include "Ball.h"
#include "Settings.h"
#include <sstream>
#include <iomanip>

std::string setPrecision(float f)
{
	std::stringstream ss;
	ss << std::setprecision(2) << f;
	return ss.str();
}

const float Ball::RADIUS = 50.f;

Ball::Ball(sf::Color color)
{
	circle.setRadius(RADIUS);
	circle.setOrigin(RADIUS, RADIUS);
	circle.setFillColor(color);

	massText.setFont(Settings::getArialFont());
	massText.setString(setPrecision(mass));
	massText.setOutlineThickness(1.f);
	massText.setOutlineColor(sf::Color(100, 100, 100));
}

void Ball::initPhysics(sf::Vector2f position, float acceleration, float mass)
{
	velocity = 0.f;
	this->acceleration = acceleration;
	this->mass = mass;

	circle.setPosition(position);

	massText.setString(setPrecision(mass));
	sf::FloatRect bounds = massText.getLocalBounds();
	massText.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
}

bool Ball::resolveCollision(Ball & other)
{
	sf::FloatRect intersection;
	if (!circle.getGlobalBounds().intersects(other.circle.getGlobalBounds(), intersection))
		return false;

	if (circle.getPosition().x > other.circle.getPosition().x)
	{
		circle.move(intersection.width / 2, 0.f);
		other.circle.move(-intersection.width / 2, 0.f);
	}
	else
	{
		circle.move(-intersection.width / 2, 0.f);
		other.circle.move(intersection.width / 2, 0.f);
	}

}

void Ball::update(float deltatime)
{
	velocity += acceleration * deltatime;
	circle.move(velocity * deltatime, 0.f);
}

void Ball::draw(sf::RenderWindow & window)
{
	massText.setPosition(circle.getPosition());
	window.draw(circle);
	window.draw(massText);
}
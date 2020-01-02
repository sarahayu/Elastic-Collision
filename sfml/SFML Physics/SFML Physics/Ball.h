#pragma once
#include <SFML\Graphics.hpp>

class Ball
{
public:

	Ball(const sf::Color &color);

	void initPhysics(const sf::Vector2f &position, const sf::Vector2f &acceleration, const float &mass);

	const bool resolveCollision(Ball &other);
	static float getRadius(const float &mass);
	const float getRadius() const;

	void update(const float &deltatime);
	void draw(sf::RenderWindow &window);

	sf::Vector2f acceleration;
	sf::Vector2f velocity;
	float mass = 0.f;

private:
	sf::CircleShape circle;
	sf::Text massText;
};

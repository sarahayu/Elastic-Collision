#pragma once
#include <SFML\Graphics.hpp>

class Ball
{
public:
	Ball(sf::Color color);

	void initPhysics(sf::Vector2f position, float acceleration, float mass);

	bool collide(Ball other);
	void resolveCollision(Ball &other);

	void update(float deltatime);
	void draw(sf::RenderWindow &window);

	float acceleration = 0.f;
	float velocity = 0.f;
	float mass = 0.f;

	static const float RADIUS;

private:
	sf::CircleShape circle;
	sf::Text massText;
};

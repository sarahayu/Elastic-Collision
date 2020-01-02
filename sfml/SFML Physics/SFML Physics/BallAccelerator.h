#pragma once
#include <SFML\Graphics.hpp>

class Ball;


class BallAccelerator
{
public:
	BallAccelerator(Ball &ball);

	void setFinalVelocity(const sf::Vector2f &finalVelocity);
	void update(float deltatime);

private:
	Ball &ballObject;
	sf::Clock clock;
	sf::Vector2f finalVelocity;
};
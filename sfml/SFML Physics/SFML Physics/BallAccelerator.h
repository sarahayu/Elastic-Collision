#pragma once
#include <SFML\Graphics.hpp>

class Ball;


class BallAccelerator
{
public:
	BallAccelerator(Ball &ball);

	void setFinalVelocity(float finalVelocity);
	void update(float deltatime);

private:
	Ball &ballObject;
	sf::Clock clock;
	float finalVelocity;
};
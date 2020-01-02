#include "BallAccelerator.h"
#include "Ball.h"
#include "Settings.h"

BallAccelerator::BallAccelerator(Ball & ball)
	: ballObject(ball)
{
}

void BallAccelerator::setFinalVelocity(const sf::Vector2f &finalVelocity)
{
	this->finalVelocity = finalVelocity;
	clock.restart();
}

void BallAccelerator::update(float deltatime)
{
	float elapsed = clock.getElapsedTime().asSeconds();
	if (elapsed > Settings::TIME_TO_COLLISION - Settings::COASTING_TIME)
	{
		ballObject.velocity = finalVelocity;
		ballObject.acceleration = sf::Vector2f();
	}
}
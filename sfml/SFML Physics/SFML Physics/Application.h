#pragma once
#include "BallAccelerator.h"
#include "Ball.h"
#include <SFML\Audio.hpp>

class Application
{
public:
	Application();

	void run();

private:

	void input();
	void update(float deltatime);
	void draw();

	void resetBalls();

	sf::Vector2u m_windowDimensions = { 800U,600U };

	sf::RenderWindow m_window;
	sf::View m_viewport;
	Ball m_ball1;
	Ball m_ball2;
	BallAccelerator m_accelerator1;
	BallAccelerator m_accelerator2;

	sf::Clock m_stopwatch;
	sf::FloatRect m_mainWindowBounds;
	sf::Sound m_boingEffect;
	sf::SoundBuffer m_soundBuffer;

	struct {
		sf::Vector2f velocity1 = { 100.f, 50.f }, velocity2 = { -50.f,-50.f };
		float mass1 = 1.f, mass2 = 1.f;
	} m_input;
};
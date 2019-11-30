#include "imgui\imgui.h"
#include "imgui\imgui-SFML.h"
#include "Settings.h"
#include "BallAccelerator.h"
#include "Ball.h"
#include <iostream>

float velocity1 = 100.f, velocity2 = -50.f;
float mass1 = 1.f, mass2 = 1.f;
unsigned int windowWidth = 800, windowHeight = 600;
sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Collision");
sf::View viewport(sf::FloatRect({}, { (float)windowWidth, (float)windowHeight }));
sf::FloatRect mainWindowBounds;
Ball ball1(sf::Color::Cyan);
Ball ball2(sf::Color::Green);
BallAccelerator accelerator1(ball1);
BallAccelerator accelerator2(ball2);
sf::Clock stopwatch;

void resetBalls();
void input(float deltatime);
void update(float deltatime);
void draw();

int main()
{
	window.setFramerateLimit(60U);
	window.setView(viewport);
	ImGui::SFML::Init(window);

	resetBalls();

	sf::Clock clock;
	stopwatch.restart();

	while (window.isOpen())
	{
		float dt = clock.restart().asSeconds();

		input(dt);
		update(dt);
		draw();
	}

	ImGui::SFML::Shutdown();
	return 0;
}

void resetBalls()
{
	float accel1 = velocity1 / Settings::ACCEL_TIME;
	float accel2 = velocity2 / Settings::ACCEL_TIME;

	float deltaX1 = 0.5f * accel1 * std::pow(Settings::ACCEL_TIME, 2) + velocity1 * Settings::COASTING_TIME;
	float deltaX2 = 0.5f * accel2 * std::pow(Settings::ACCEL_TIME, 2) + velocity2 * Settings::COASTING_TIME;

	ball1.initPhysics({ windowWidth / 2 - Ball::RADIUS - deltaX1,(float)windowHeight / 2 }, accel1, mass1);
	ball2.initPhysics({ windowWidth / 2 + Ball::RADIUS - deltaX2,(float)windowHeight / 2 }, accel2, mass2);

	accelerator1.setFinalVelocity(velocity1);
	accelerator2.setFinalVelocity(velocity2);
}

void input(float deltatime)
{
	sf::Event evnt;

	auto checkInput = [&]() {

		if (velocity1 < velocity2)
		{
			std::swap(velocity1, velocity2);
			std::swap(mass1, mass2);
		}
		else if (velocity1 == velocity2)
			velocity2--;

		resetBalls();

		stopwatch.restart();
	};

	while (window.pollEvent(evnt))
	{
		ImGui::SFML::ProcessEvent(evnt);

		switch (evnt.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::Resized:
			windowWidth = evnt.size.width;
			windowHeight = evnt.size.height;
			viewport.setSize((float)windowWidth, (float)windowHeight);
			viewport.setCenter((float)windowWidth / 2, (float)windowHeight / 2);
			window.setView(viewport);
			break;
		case sf::Event::KeyReleased:
			if (evnt.key.code == sf::Keyboard::Enter) checkInput();
			break;
		}
	}


	ImGui::SFML::Update(window, sf::seconds(deltatime));

	ImGui::Begin("Perfectly Elastic Collision", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Ball 1");
	ImGui::Spacing();
	ImGui::Text(" Velocity"); ImGui::SameLine(100.f); ImGui::InputFloat("##1", &velocity1, 1.0f, 1.0f, "%.2f", ImGuiInputTextFlags_CharsDecimal);
	ImGui::Text(" Mass"); ImGui::SameLine(100.f); ImGui::InputFloat("##2", &mass1, 1.0f, 1.0f, "%.1f", ImGuiInputTextFlags_CharsDecimal);
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Text("Ball 2");
	ImGui::Spacing();
	ImGui::Text(" Velocity"); ImGui::SameLine(100.f); ImGui::InputFloat("##3", &velocity2, 1.0f, 1.0f, "%.2f", ImGuiInputTextFlags_CharsDecimal);
	ImGui::Text(" Mass"); ImGui::SameLine(100.f); ImGui::InputFloat("##4", &mass2, 1.0f, 1.0f, "%.1f", ImGuiInputTextFlags_CharsDecimal);
	ImGui::Spacing();
	ImGui::Separator();
	mass1 = std::max(0.1f, mass1); mass2 = std::max(0.1f, mass2);

	ImGui::Spacing();
	ImGui::Text("%.2fs", stopwatch.getElapsedTime().asSeconds());
	mainWindowBounds = sf::FloatRect(ImGui::GetWindowPos(), ImGui::GetWindowSize());
	ImGui::SameLine(mainWindowBounds.width - 91);
	if (ImGui::Button("Reset demo")) checkInput();
	ImGui::Spacing();
	ImGui::End();
}

void update(float deltatime)
{
	float elapsed = stopwatch.getElapsedTime().asSeconds();
	if (elapsed < Settings::TIME_TO_COLLISION)
	{
		accelerator1.update(deltatime);
		accelerator2.update(deltatime);
	}
	ball1.update(deltatime);
	ball2.update(deltatime);

	if (ball1.collide(ball2))
	{
		ball1.resolveCollision(ball2);

		float a = 0.5f * mass2 + std::pow(mass2, 2) / (2 * mass1);
		float b = -2 * (mass1*velocity1 + mass2*velocity2)*mass2 / (2 * mass1);
		float c = (mass1*velocity1 + mass2*velocity2) / (2 * mass1) - 0.5f*mass1*std::pow(velocity1, 2)
			- 0.5f*mass2*std::pow(velocity2, 2);

		ball2.velocity = (-b + std::sqrt(std::pow(b, 2) - 4 * a*c)) / (2 * a);
		ball1.velocity = (mass1*velocity1 + mass2*velocity2 - mass2 * ball2.velocity) / mass1;
	}

	if (elapsed > Settings::TIME_TO_COLLISION)
	{
		if (std::abs(elapsed - Settings::TIME_TO_COLLISION) < 0.1f)
			ImGui::SetNextWindowPos({ mainWindowBounds.left + mainWindowBounds.width,mainWindowBounds.top });

		ImGui::Begin("Final Velocities", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("Ball 1"); ImGui::SameLine(mainWindowBounds.width - 92); ImGui::Text("%.2f", ball1.velocity);
		ImGui::Text("Ball 2"); ImGui::SameLine(mainWindowBounds.width - 92); ImGui::Text("%.2f", ball2.velocity);
		ImGui::End();
	}
}

void draw()
{
	window.clear(sf::Color(230,230,230));
	ball1.draw(window);
	ball2.draw(window);
	ImGui::SFML::Render(window);
	window.display();
}
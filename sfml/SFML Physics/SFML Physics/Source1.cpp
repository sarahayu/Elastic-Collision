#include <SFML\Graphics.hpp>
#include "imgui\imgui.h"
#include "imgui\imgui-SFML.h"
#include <iostream>

const float TIME_TO_COLLISION = 2.f;
const float COASTING_TIME = 0.2f;
const float ACCEL_TIME = TIME_TO_COLLISION - COASTING_TIME;
sf::Font ARIAL_FONT;

std::string cutZeroes(std::string str)
{
	// remove trailing zeroes: refer to https://stackoverflow.com/a/13709929
	str.erase(str.find_last_not_of('0') + 1, std::string::npos);
	str.erase(str.find_last_not_of('.') + 1, std::string::npos);

	return str;
}

class Ball
{
public:
	Ball(float acceleration, float finalVelocity, sf::Vector2f position, sf::Color color, float mass)
	{
		this->acceleration = acceleration;
		this->velocity = 0.f;
		this->finalVelocity = finalVelocity;

		circle.setPosition(position);
		circle.setRadius(50.f);
		circle.setOrigin(50.f, 50.f);
		circle.setFillColor(color);

		massText.setFont(ARIAL_FONT);
		massText.setString(cutZeroes(std::to_string(mass)));
		massText.setCharacterSize(60U);
		massText.setOutlineThickness(1.f);
		sf::FloatRect bounds = massText.getLocalBounds();
		massText.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
	}

	bool collide(Ball other)
	{
		return circle.getGlobalBounds().intersects(other.circle.getGlobalBounds());
	}

	void resolveCollision(Ball &other)
	{
		sf::FloatRect intersection;
		circle.getGlobalBounds().intersects(other.circle.getGlobalBounds(), intersection);
		if (circle.getPosition().x > other.circle.getPosition().x) circle.move(intersection.width, 0.f);
		else other.circle.move(intersection.width, 0.f);

	}

	void update(float deltatime)
	{
		float elapsed = clock.getElapsedTime().asSeconds();
		if (elapsed < TIME_TO_COLLISION - COASTING_TIME)
			velocity += acceleration * deltatime;
		else if (elapsed < TIME_TO_COLLISION)
			velocity = finalVelocity;

		circle.move(velocity * deltatime, 0.f);
	}

	void draw(sf::RenderWindow &window)
	{
		massText.setPosition(circle.getPosition());
		window.draw(circle);
		window.draw(massText);
	}

	float acceleration;
	float velocity;

private:
	sf::CircleShape circle;
	sf::Text massText;
	sf::Clock clock;
	float finalVelocity;
};



int main()
{
	ARIAL_FONT.loadFromFile("arial.ttf");
	sf::RenderWindow window(sf::VideoMode(800, 600), "Collision");
	window.setFramerateLimit(60U);
	ImGui::SFML::Init(window);
	ImGui::SetNextWindowPos({ 0.f,0.f });

	float velocity1 = 500.f, velocity2 = 500.f;
	float mass1 = 5.f, mass2 = 5.f;

	float accel1 = velocity1 / ACCEL_TIME;
	float accel2 = velocity2 / ACCEL_TIME;

	float deltaX1 = 0.5f * accel1 * std::pow(ACCEL_TIME, 2) + velocity1 * COASTING_TIME;
	float deltaX2 = 0.5f * accel2 * std::pow(ACCEL_TIME, 2) + velocity2 * COASTING_TIME;

	Ball ball1(accel1, velocity1, { 400.f - 50.f - deltaX1,300.f }, sf::Color::Cyan, mass1);
	Ball ball2(-accel2, -velocity2, { 400.f + 50.f + deltaX2,300.f }, sf::Color::Green, mass2);

	sf::Clock clock;
	sf::Clock stopwatch;

	while (window.isOpen())
	{
		sf::Event evnt;

		while (window.pollEvent(evnt))
		{
			ImGui::SFML::ProcessEvent(evnt);

			switch (evnt.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}

		float dt = clock.restart().asSeconds();

		ImGui::SFML::Update(window, sf::seconds(dt));

		ImGui::Begin("Perfectly Elastic Collision", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::PushItemWidth(100);
		ImGui::InputFloat("Ball 1 velocity", &velocity1, 1.0f, 1.0f, "%.1f");
		ImGui::InputFloat("Ball 1 mass", &mass1, 1.0f, 1.0f, "%.1f");
		ImGui::InputFloat("Ball 2 velocity", &velocity2, 1.0f, 1.0f, "%.1f");
		ImGui::InputFloat("Ball 2 mass", &mass2, 1.0f, 1.0f, "%.1f");

		if (ImGui::Button("Reset demo"))
		{
			std::cout << "\nReset demo";
		}

		ImGui::SameLine(100);
		ImGui::Text(std::to_string(stopwatch.getElapsedTime().asSeconds()).c_str());

		ImGui::End();

		ball1.update(dt);
		ball2.update(dt);

		if (ball1.collide(ball2))
		{
			ball1.resolveCollision(ball2);

			ball1.acceleration = -ball1.acceleration;
			ball1.velocity = -ball1.velocity;
			ball2.acceleration = -ball2.acceleration;
			ball2.velocity = -ball2.velocity;
		}

		window.clear(sf::Color::White);
		ball1.draw(window);
		ball2.draw(window);
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
	return 0;
}
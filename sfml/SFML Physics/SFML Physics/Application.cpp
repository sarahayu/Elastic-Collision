#include "Application.h"
#include "Settings.h"
#include "imgui\imgui.h"
#include "imgui\imgui-SFML.h"
#include <iostream>

Application::Application()
	:
	m_window(sf::VideoMode(m_windowDimensions.x, m_windowDimensions.y), "Collision"),
	m_viewport(sf::FloatRect({},(sf::Vector2f)(m_windowDimensions))),
	m_ball1(sf::Color::Cyan * sf::Color(200, 200, 200)),
	m_ball2(sf::Color::Green * sf::Color(200, 200, 200)),
	m_accelerator1(m_ball1),
	m_accelerator2(m_ball2)
{
	m_soundBuffer.loadFromFile("boing.ogg");
	m_boingEffect.setBuffer(m_soundBuffer);

	m_window.setView(m_viewport);
	ImGui::SFML::Init(m_window);

	std::cout << "Created by Sarah Yuniar & Ahmed Raiyan";

	resetBalls();
}

void Application::run()
{
	sf::Clock clock;
	m_stopwatch.restart();
	float accumulatedDT = 0.f;
	const float frameDuration = 1.f / 120;

	while (m_window.isOpen())
	{
		float dt = clock.restart().asSeconds();
		accumulatedDT += dt;

		ImGui::SFML::Update(m_window, sf::seconds(dt));
		input();

		while (accumulatedDT >= frameDuration)
		{
			accumulatedDT -= frameDuration;
			update(frameDuration);
		}
		draw();
	}

	ImGui::SFML::Shutdown();
}

void Application::input()
{
	sf::Event evnt;

	auto checkInput = [&]() {

		if (m_input.velocity1.x < m_input.velocity2.x)
		{
			std::swap(m_input.velocity1, m_input.velocity2);
			std::swap(m_input.mass1, m_input.mass2);
		}
		else if (m_input.velocity1 == m_input.velocity2)
			m_input.velocity2 = { m_input.velocity2.x - 1,m_input.velocity2.y - 1 };

		resetBalls();

		m_stopwatch.restart();
	};

	while (m_window.pollEvent(evnt))
	{
		ImGui::SFML::ProcessEvent(evnt);

		switch (evnt.type)
		{
		case sf::Event::Closed:
			m_window.close();
			break;
		case sf::Event::Resized:
			m_windowDimensions.x = evnt.size.width;
			m_windowDimensions.y = evnt.size.height;
			m_viewport.setSize((float)m_windowDimensions.x, (float)m_windowDimensions.y);
			m_viewport.setCenter((float)m_windowDimensions.x / 2, (float)m_windowDimensions.y / 2);
			m_window.setView(m_viewport);
			break;
		case sf::Event::KeyReleased:
			if (evnt.key.code == sf::Keyboard::Enter) checkInput();
			break;
		}
	}

	ImGui::Begin("Perfectly Elastic Collision", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Ball 1"); ImGui::Spacing();
	ImGui::Text(" Velocity"); ImGui::SameLine(100.f); 
	ImGui::InputFloat("##1", &m_input.velocity1.x, 1.0f, 1.0f, "%.2f");
	ImGui::Text(" Mass"); ImGui::SameLine(100.f); 
	ImGui::InputFloat("##2", &m_input.mass1, 1.0f, 1.0f, "%.1f");
	ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
	ImGui::Text("Ball 2"); ImGui::Spacing();
	ImGui::Text(" Velocity"); ImGui::SameLine(100.f); 
	ImGui::InputFloat("##3", &m_input.velocity2.x, 1.0f, 1.0f, "%.2f");
	ImGui::Text(" Mass"); ImGui::SameLine(100.f); 
	ImGui::InputFloat("##4", &m_input.mass2, 1.0f, 1.0f, "%.1f");
	ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
	
	m_input.mass1 = std::max(0.1f, m_input.mass1); m_input.mass2 = std::max(0.1f, m_input.mass2);
	
	ImGui::Text("%.2fs", m_stopwatch.getElapsedTime().asSeconds());
	m_mainWindowBounds = sf::FloatRect(ImGui::GetWindowPos(), ImGui::GetWindowSize());
	ImGui::SameLine(m_mainWindowBounds.width - 91);
	if (ImGui::Button("Reset demo")) checkInput();
	ImGui::Spacing();

	ImGui::End();

	float elapsed = m_stopwatch.getElapsedTime().asSeconds();
	if (elapsed > Settings::TIME_TO_COLLISION)
	{
		if (std::abs(elapsed - Settings::TIME_TO_COLLISION) < 0.1f)
			ImGui::SetNextWindowPos({ m_mainWindowBounds.left + m_mainWindowBounds.width,m_mainWindowBounds.top });

		ImGui::Begin("Final Velocities", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("Ball 1"); ImGui::SameLine(m_mainWindowBounds.width - 92); ImGui::Text("%.2f", m_ball1.velocity.x);
		ImGui::Text("Ball 2"); ImGui::SameLine(m_mainWindowBounds.width - 92); ImGui::Text("%.2f", m_ball2.velocity.x);
		ImGui::End();
	}
}

void Application::update(float deltatime)
{
	if (m_stopwatch.getElapsedTime().asSeconds() < Settings::TIME_TO_COLLISION)
	{
		m_accelerator1.update(deltatime);
		m_accelerator2.update(deltatime);
	}
	m_ball1.update(deltatime);
	m_ball2.update(deltatime);

	if (m_ball1.resolveCollision(m_ball2))
	{
		float a = 0.5f *m_input.mass2 + std::pow(m_input.mass2, 2) / (2 * m_input.mass1);
		float b = -2 * (m_input.mass1*m_input.velocity1.x + m_input.mass2*m_input.velocity2.x)*m_input.mass2 / (2 * m_input.mass1);
		float c = std::pow((m_input.mass1*m_input.velocity1.x + m_input.mass2*m_input.velocity2.x), 2) / (2 * m_input.mass1)
			- 0.5f*m_input.mass1*std::pow(m_input.velocity1.x, 2)	- 0.5f*m_input.mass2*std::pow(m_input.velocity2.x, 2);

		m_ball2.velocity.x = (-b + std::sqrt(std::pow(b, 2) - 4 * a*c)) / (2 * a);
		m_ball1.velocity.x = (m_input.mass1*m_input.velocity1.x + m_input.mass2*m_input.velocity2.x - m_input.mass2 * m_ball2.velocity.x) / m_input.mass1;

		m_boingEffect.play();
	}
}

void Application::draw()
{
	m_window.clear(sf::Color(230, 230, 230));
	m_ball1.draw(m_window);
	m_ball2.draw(m_window);
	ImGui::SFML::Render();
	m_window.display();
}

void Application::resetBalls()
{
	sf::Vector2f accel1 = m_input.velocity1 / Settings::ACCEL_TIME;
	sf::Vector2f accel2 = m_input.velocity2 / Settings::ACCEL_TIME;

	sf::Vector2f delta1 = 0.5f * accel1 * std::pow(Settings::ACCEL_TIME, 2) + m_input.velocity1 * Settings::COASTING_TIME;
	sf::Vector2f delta2 = 0.5f * accel2 * std::pow(Settings::ACCEL_TIME, 2) + m_input.velocity2 * Settings::COASTING_TIME;

	m_ball1.initPhysics((sf::Vector2f)m_windowDimensions / 2.f - sf::Vector2f(Ball::getRadius(m_input.mass1), 0.f) - delta1, 
		accel1, m_input.mass1);
	m_ball2.initPhysics((sf::Vector2f)m_windowDimensions / 2.f + sf::Vector2f(Ball::getRadius(m_input.mass2), 0.f) - delta2, 
		accel2, m_input.mass2);

	m_accelerator1.setFinalVelocity(m_input.velocity1);
	m_accelerator2.setFinalVelocity(m_input.velocity2);
}

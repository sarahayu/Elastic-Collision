#include "Application.h"
#include "Settings.h"
#include "imgui\imgui.h"
#include "imgui\imgui-SFML.h"
#include "util\MathUtil.h"
#include <iostream>

Application::Application()
	:
	m_window(sf::VideoMode(m_windowDimensions.x, m_windowDimensions.y), "Collision"),
	m_viewport(sf::FloatRect(sf::Vector2f(0.f, -(int)m_windowDimensions.y),sf::Vector2f(m_windowDimensions.x, m_windowDimensions.y))),
	m_ball1(sf::Color::Cyan * sf::Color(200, 200, 200)),
	m_ball2(sf::Color::Green * sf::Color(200, 200, 200)),
	m_accelerator1(m_ball1),
	m_accelerator2(m_ball2)
{
	m_soundBuffer.loadFromFile("boing.ogg");
	m_boingEffect.setBuffer(m_soundBuffer);
	
	ImGui::SFML::Init(m_window);

	// add arial font to support theta symbol
	ImFontConfig config;
	config.MergeMode = true;
	auto io = ImGui::GetIO();
	//fonts.push_back(io.Fonts->AddFontFromFileTTF("roboto.ttf", 16.f));
	builder.AddChar(0x03B8);		// theta symbol
	builder.AddChar(0x00B0);		// degree symbol
	builder.BuildRanges(&ranges);
	fonts.push_back(io.Fonts->AddFontFromFileTTF("arial.ttf", 16.f, &config, ranges.Data));
	ImGui::SFML::UpdateFontTexture();

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
	std::string errors;
	bool startSimulation = false;

	auto checkInput = [&]() {
/*
		if (m_input.velocity1.x < m_input.velocity2.x)
		{
			std::swap(m_input.velocity1, m_input.velocity2);
			std::swap(m_input.mass1, m_input.mass2);
		}
		else if (m_input.velocity1 == m_input.velocity2)
			m_input.velocity2 = { m_input.velocity2.x - 1,m_input.velocity2.y - 1 };*/

		if (errors.length() == 0)
		{
			resetBalls();

			m_stopwatch.restart();
		}
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
			m_viewport.reset(sf::FloatRect(sf::Vector2f(0.f, -(int)m_windowDimensions.y), sf::Vector2f(m_windowDimensions.x, m_windowDimensions.y)));
			break;
		case sf::Event::KeyReleased:
			if (evnt.key.code == sf::Keyboard::Enter) startSimulation = true;
			break;
		}
	}


	ImGui::Begin("Perfectly Elastic Collision", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Ball 1"); ImGui::Spacing();
	ImGui::Text(" Velocity"); 
	ImGui::SameLine(100.f); ImGui::SetNextItemWidth(70.f); ImGui::InputFloat(m_input.usingPolarCoords ? "r##1" : "x##1", &m_input.velocity1.x, 0.0f, 0.0f, "%.2f");
	ImGui::SameLine(0.f, 2.f); ImGui::SetNextItemWidth(70.f); ImGui::InputFloat(m_input.usingPolarCoords ? u8"θ##1" : "y##1", &m_input.velocity1.y, 0.0f, 0.0f, "%.2f");

	ImGui::Text(" Mass"); 
	ImGui::SameLine(100.f); ImGui::SetNextItemWidth(70.f); ImGui::InputFloat("##3", &m_input.mass1, 0.0f, 0.0f, "%.1f");
	ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
	
	ImGui::Text("Ball 2"); ImGui::Spacing();
	ImGui::Text(" Velocity");
	ImGui::SameLine(100.f); ImGui::SetNextItemWidth(70.f); ImGui::InputFloat(m_input.usingPolarCoords ? "r##2" : "x##2", &m_input.velocity2.x, 0.0f, 0.0f, "%.2f");
	ImGui::SameLine(0.f, 2.f); ImGui::SetNextItemWidth(70.f); ImGui::InputFloat(m_input.usingPolarCoords ? u8"θ##2" : "y##2", &m_input.velocity2.y, 0.0f, 0.0f, "%.2f");

	ImGui::Text(" Mass");
	ImGui::SameLine(100.f); ImGui::SetNextItemWidth(70.f); ImGui::InputFloat("##6", &m_input.mass2, 0.0f, 0.0f, "%.1f");
	ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
	
	//m_input.mass1 = std::max(0.1f, m_input.mass1); m_input.mass2 = std::max(0.1f, m_input.mass2);

	if (m_input.mass1 <= 0.1f) errors += "Mass 1 too small! Must be greater than 0.1\n";
	if (m_input.mass2 <= 0.1f) errors += "Mass 2 too small! Must be greater than 0.1\n";
	
	if (ImGui::Checkbox("Polar Coordinates", &m_input.usingPolarCoords))
	{
		if (m_input.usingPolarCoords)
		{
			m_input.velocity1 = mu::rectangularToPolar(m_input.velocity1);
			m_input.velocity2 = mu::rectangularToPolar(m_input.velocity2);
		}
		else
		{
			m_input.velocity1 = mu::polarToRectangular(m_input.velocity1);
			m_input.velocity2 = mu::polarToRectangular(m_input.velocity2);
		}
	}

	if (errors.length() != 0)
	{
		ImGui::Spacing(); ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), errors.c_str()); ImGui::Spacing();
	}

	ImGui::Text("%.2fs", m_stopwatch.getElapsedTime().asSeconds());
	m_mainWindowBounds = sf::FloatRect(ImGui::GetWindowPos(), ImGui::GetWindowSize());
	ImGui::SameLine(m_mainWindowBounds.width - 91);
	if (ImGui::Button("Reset demo")) startSimulation = true;
	ImGui::Spacing();

	ImGui::End();

	if (startSimulation)
	{
		if (errors.length() == 0)
		{
			resetBalls();

			m_stopwatch.restart();
		}
	}

	float elapsed = m_stopwatch.getElapsedTime().asSeconds();
	if (elapsed > Settings::TIME_TO_COLLISION)
	{
		if (std::abs(elapsed - Settings::TIME_TO_COLLISION) < 0.1f)
			ImGui::SetNextWindowPos({ m_mainWindowBounds.left + m_mainWindowBounds.width,m_mainWindowBounds.top });

		char *degree = u8"";
		sf::Vector2f velocity1 = m_ball1.velocity, velocity2 = m_ball2.velocity;
		if (m_input.usingPolarCoords)
		{
			velocity1 = mu::rectangularToPolar(velocity1);
			velocity2 = mu::rectangularToPolar(velocity2);
			degree = u8"°";
		}

		ImGui::Begin("Final Velocities", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("Ball 1"); 
		ImGui::SameLine(100.f); ImGui::Text("(%.2f,%.2f%s)", velocity1.x, velocity1.y, degree);
		ImGui::Text("Ball 2"); 
		ImGui::SameLine(100.f); ImGui::Text("(%.2f,%.2f%s)", velocity2.x, velocity2.y, degree);
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
		sf::Vector2f velocity1 = m_input.velocity1, velocity2 = m_input.velocity2;
		float mass1 = m_ball1.getMass(), mass2 = m_ball2.getMass();
		if (m_input.usingPolarCoords)
		{
			velocity1 = mu::polarToRectangular(velocity1);
			velocity2 = mu::polarToRectangular(velocity2);
		}

		m_ball1.velocity.x = (velocity1.x * (mass1 - mass2)	+ 2 * mass2 * velocity2.x)
			/ (mass1 + mass2);
		m_ball2.velocity.x = (velocity2.x * (mass2 - mass1) + 2 * mass1 * velocity1.x) 
			/ (mass1 + mass2);

		//m_boingEffect.play();
	}
}

void Application::draw()
{
	m_window.clear(sf::Color(230, 230, 230));
	m_window.setView(m_viewport);
	m_ball1.draw(m_window);
	m_ball2.draw(m_window);
	//ImGui::ShowDemoWindow();
	ImGui::SFML::Render();
	m_window.display();
}

void Application::resetBalls()
{
	sf::Vector2f velocity1 = m_input.velocity1, velocity2 = m_input.velocity2;
	if (m_input.usingPolarCoords)
	{
		velocity1 = mu::polarToRectangular(velocity1);
		velocity2 = mu::polarToRectangular(velocity2);
	}

	sf::Vector2f accel1 = velocity1 / Settings::ACCEL_TIME;
	sf::Vector2f accel2 = velocity2 / Settings::ACCEL_TIME;

	sf::Vector2f delta1 = 0.5f * accel1 * std::pow(Settings::ACCEL_TIME, 2) + velocity1 * Settings::COASTING_TIME;
	sf::Vector2f delta2 = 0.5f * accel2 * std::pow(Settings::ACCEL_TIME, 2) + velocity2 * Settings::COASTING_TIME;

	m_ball1.initPhysics((sf::Vector2f)m_windowDimensions / 2.f - sf::Vector2f(Ball::getRadius(m_input.mass1), 0.f) - delta1, 
		accel1, m_input.mass1);
	m_ball2.initPhysics((sf::Vector2f)m_windowDimensions / 2.f + sf::Vector2f(Ball::getRadius(m_input.mass2), 0.f) - delta2, 
		accel2, m_input.mass2);

	m_accelerator1.setFinalVelocity(velocity1);
	m_accelerator2.setFinalVelocity(velocity2);
}

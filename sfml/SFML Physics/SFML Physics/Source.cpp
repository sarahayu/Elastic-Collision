//#include <SFML\Graphics.hpp>
//#include <iostream>
//
//// refer to this https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331
//
//class Ball
//{
//public:
//	Ball(const sf::Vector2f &position, const float &vel, const float &accel, const sf::Color &color, const float &radius = 50.f)
//	{
//		velocity = vel;
//		acceleration = accel;
//		m_circle.setRadius(radius);
//		m_circle.setOrigin(radius, radius);
//		m_circle.setPosition(position);
//		m_circle.setFillColor(color);
//	}
//
//	void update(const float &deltatime)
//	{
//		velocity += acceleration * deltatime;
//		m_circle.move(velocity*deltatime, 0.f);
//	}
//
//	friend const bool collided(const Ball &first, const Ball &second)
//	{
//		return first.m_circle.getGlobalBounds().intersects(second.m_circle.getGlobalBounds());
//	}
//
//	//naive collision resolution
//	friend void resolveCollision(Ball &first, Ball &second)
//	{
//		sf::FloatRect intersection;
//		sf::CircleShape &firstCircle = first.m_circle, &secondCircle = second.m_circle;
//		firstCircle.getGlobalBounds().intersects(secondCircle.getGlobalBounds(), intersection);
//		if (firstCircle.getPosition().x < secondCircle.getPosition().x) secondCircle.move(intersection.width, 0.f);
//		else firstCircle.move(intersection.width, 0.f);
//	}
//
//	void draw(sf::RenderWindow &window) const
//	{
//		window.draw(m_circle);
//	}
//
//	float velocity;
//	float acceleration;
//
//private:
//	sf::CircleShape m_circle;
//};
//
//int main()
//{
//	sf::RenderWindow window(sf::VideoMode(800,600), "Physics");
//	sf::Clock clock;
//	Ball ball({ 0.f,300.f }, 0.f, 800.f, sf::Color::Cyan);
//	Ball ball2({ 800.f,300.f }, 0.f, -800.f, sf::Color::Red);
//
//	while (window.isOpen())
//	{
//		sf::Event evnt;
//		 
//		while (window.pollEvent(evnt))
//		{
//			switch (evnt.type)
//			{
//			case sf::Event::Closed:
//				window.close();
//				break;
//			}
//		}
//
//		const float dt = clock.restart().asSeconds();
//		ball.update(dt);
//		ball2.update(dt);
//
//		if (collided(ball, ball2))
//		{
//			resolveCollision(ball, ball2);
//			ball.acceleration = -ball.acceleration;
//			ball.velocity = -ball.velocity;
//			ball2.acceleration = -ball2.acceleration;
//			ball2.velocity = -ball2.velocity;
//		}
//
//		window.clear(sf::Color::White);
//		ball.draw(window);
//		ball2.draw(window);
//		window.display();
//	}
//
//	return 0;
//}
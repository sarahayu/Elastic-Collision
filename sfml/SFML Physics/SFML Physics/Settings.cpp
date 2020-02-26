#include "Settings.h"
#include <iostream>

namespace
{
	sf::Font getFont(const std::string &filename)
	{
		sf::Font font;
		font.loadFromFile(filename);
		return font;
	}
}


sf::Font & Settings::getComfortaaFont()
{
	static sf::Font arial = getFont("comfortaa.ttf");

	return arial;
}

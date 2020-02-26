#include "Settings.h"

sf::Font & Settings::getArialFont()
{
	static sf::Font arial;
	arial.loadFromFile("arial.ttf");
	return arial;
}

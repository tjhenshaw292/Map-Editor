#include "stdafx.h"
#include "MapMaker.h"

int main()
{
	MapMaker makeMe("pokeTiles2.png", sf::Vector2u(16, 16), 640, 640);
	makeMe.load("home.dat");
	makeMe.display();
	makeMe.save("home.dat");
	return 0;
}
#include "stdafx.h"
#include "MapMaker.h"

int main()
{
	if (!MapMaker::mainFont.loadFromFile("Carlito-Regular.ttf"))
		throw;
	MapMaker makeMe("pokeTiles3.png", sf::Vector2u(16, 16), 2400, 1760);
	//makeMe.load("newArea.dat");
	makeMe.display();
	makeMe.save("newArea.dat");
	return 0;
}
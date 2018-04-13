#include "stdafx.h"
#include "MapMaker.h"

int main()
{
	if (!MapMaker::mainFont.loadFromFile("Carlito-Regular.ttf"))
		throw;
	MapMaker makeMe("pokeTiles2.png", sf::Vector2u(16, 16), 240, 160);
	//makeMe.load("newArea.dat");
	makeMe.display();
	makeMe.save("newArea.dat");
	return 0;
}
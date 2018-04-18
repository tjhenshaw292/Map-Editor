#include "stdafx.h"
#include "MapMaker.h"

int main()
{
	if (!MapMaker::mainFont.loadFromFile("Carlito-Regular.ttf"))
		throw;
	MapMaker makeMe("pokeTiles3.png", sf::Vector2u(16, 16), 24, 17);
	makeMe.setBlankTile(6071);
	makeMe.setMapFileName("ImmaRealBoy.dat");
	//makeMe.load("newArea.dat");
	makeMe.display();
	makeMe.save("ImmaRealBoy.dat");
	return 0;
}
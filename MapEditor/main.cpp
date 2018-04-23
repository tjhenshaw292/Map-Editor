#include "stdafx.h"
#include "MapMaker.h"

int main()
{
	if (!MapMaker::mainFont.loadFromFile("Carlito-Regular.ttf"))
		throw;
	MapMaker makeMe("pokeTiles2.png", sf::Vector2u(16, 16), 32, 69);
	makeMe.setBlankTile(551);
	makeMe.setMapFileName("ImmaRealBoy.dat");
	makeMe.load("ImmaRealBoy.dat");
	makeMe.display();
	makeMe.save("ImmaRealBoy.dat");
	return 0;
}
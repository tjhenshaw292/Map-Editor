#include "stdafx.h"
#include "MapMaker.h"

int main()
{
	MapMaker makeMe("pokeTiles2.png", sf::Vector2u(16, 16), 800, 352);
	makeMe.load("firstTown.dat");
	makeMe.display();
	makeMe.save("firstTown.dat");
	return 0;
}
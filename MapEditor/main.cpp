#include "stdafx.h"
#include "MapMaker.h"

int main()
{
	MapMaker makeMe("pokeTiles2.png", sf::Vector2u(16, 16), 160, 144);
	makeMe.load("pokeMart.dat");
	makeMe.display();
	makeMe.save("pokeMart.dat");
	return 0;
}
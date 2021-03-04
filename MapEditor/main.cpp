#include "stdafx.h"
#include "MapMaker.h"

int main()
{
	if (!MapMaker::mainFont.loadFromFile("Carlito-Regular.ttf"))
		throw;
	MapMaker makeMe("gen1.png", sf::Vector2u(16, 16), 150, 150);
	makeMe.setBlankTile(551);

	std::string fileName{ "abyss.dat" };

	makeMe.setMapFileName(fileName);
	//makeMe.load(fileName);

	//In order to use this, just add the row/col then reOpen mapmaker
	//makeMe.addRowToTop(1);
	//makeMe.addColumnToLeft(1);
	//makeMe.addColumnToRight(1);

	makeMe.display();
	makeMe.save(fileName);
	return 0;
}
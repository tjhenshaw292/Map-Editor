#include "stdafx.h"
#include "MapMaker.h"

int main()
{
	if (!MapMaker::mainFont.loadFromFile("Carlito-Regular.ttf"))
		throw;
	MapMaker makeMe("gen3Indoors.png", sf::Vector2u(16, 16), 13, 7);
	makeMe.setBlankTile(0); //551 for gen 1.....0 for gen 3

	std::string fileName{ "treasureCave.dat" };

	makeMe.setMapFileName(fileName);
	makeMe.load(fileName);

	//In order to use this, just add the row/col then reOpen mapmaker
	//makeMe.addRowToTop(1);
	//makeMe.addColumnToLeft(1);
	//makeMe.addColumnToRight(5);
	//makeMe.addRowToBottow(1);  232 x 163	171 x 145

	makeMe.display();
	makeMe.save(fileName);
	return 0;
}
#include "stdafx.h"
#include "MapMaker.h"

int main()
{
	if (!MapMaker::mainFont.loadFromFile("Carlito-Regular.ttf"))
		throw;
	MapMaker makeMe("gen1.png", sf::Vector2u(16, 16), 11, 58);
	makeMe.setBlankTile(551); //551 for gen 1.....0 for gen 3

	std::string fileName{ "cave02.dat" };

	makeMe.setMapFileName(fileName);
	makeMe.load(fileName);

	//In order to use this, just add the row/col then reOpen mapmaker
	//makeMe.addRowToTop(1);
	//makeMe.addColumnToLeft(1);
	//makeMe.addColumnToRight(1);
	//makeMe.addRowToBottow(1);  192.144.96	200.160.112 144.152.152 160.160.160 136.96.64 160.128.80 232.232.184 200.200.160

	makeMe.display();
	makeMe.save(fileName);
	return 0;
}
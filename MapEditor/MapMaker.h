#pragma once
#include "TileMap.h"

class TaskWindow;
//maybe show selected tile

class MapMaker
{
	TaskWindow *m_taskWindow;

	sf::Texture m_tileSet;
	std::string m_tileSetName;
	sf::RenderWindow m_window;
	sf::Vector2u m_tileSize;
	TileMap m_tileSheet;
	TileMap m_foreground;
	TileMap m_background;
	TileMap m_mask;
	sf::VertexArray m_lines;
	std::vector<sf::Text> m_letters;
	unsigned int m_windowHeight;
	int m_selectedTile;
	//bool drawLines;
	bool drawBackground;
	bool drawForeground;
	bool drawMask;
	bool drawLetters;

	void createLines();
	void createLetters();
	void handleClick(sf::Mouse::Button button, sf::Vector2i &position);
	void handleKeyPress(sf::Event &eventy);
	void handleKeyHold(sf::Keyboard::Key key, sf::Vector2i &position);
	void setProperty(sf::Text &letter, TileMap::TileProperty prop);


public:
	MapMaker(std::string tileSetName, sf::Vector2u tileSize, unsigned int mapWidth, unsigned int mapHeight);
	void display();
	void save(std::string fileName);
	void load(std::string fileName);
	static sf::Font mainFont;
	~MapMaker();
};
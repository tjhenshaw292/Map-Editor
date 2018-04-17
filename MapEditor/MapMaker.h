#pragma once
#include "TileMap.h"

class TaskWindow;
//maybe show selected tile

sf::Clock MasterClock;

enum Direction
{
	UP, DOWN, LEFT, RIGHT
};

class MovableScreen
{
	sf::View m_view;
	sf::Vector2u m_size;
	static float s_screenSpeed;
public:
	MovableScreen(sf::Vector2u &screenSize);
	void assignTileMap(TileMap &map);
	bool canMove(Direction);
	void move(Direction, float distance);
	void move(Direction);
	void zoom(float amount);
	const sf::View& getView();
	void setViewport(sf::FloatRect &port);
};


class MapMaker
{
	TaskWindow *m_taskWindow;
	sf::FloatRect m_mapViewport;
	sf::FloatRect m_tileViewport;
	MovableScreen m_mapScreen;
	MovableScreen m_tileScreen;

	sf::Texture m_tileSet;
	std::string m_tileSetName;
	sf::RenderWindow m_window;
	sf::Vector2u m_tileSize;
	TileMap m_tileSheet;
	TileMap m_foreground;
	TileMap m_background;
	TileMap m_mask;
	sf::VertexArray m_mapLines;
	sf::VertexArray m_tileLines;
	std::vector<sf::Text> m_letters;
	unsigned int m_windowHeight;
	int m_selectedTile;

	void createLines();
	void createLetters();
	void handleClick(sf::Mouse::Button button, sf::Vector2i &position);
	void handleKeyPress(sf::Event &eventy);
	void handleKeyHold(sf::Keyboard::Key key, sf::Vector2i &position);
	void handleMouseScroll(sf::Event &eventy);
	void setProperty(sf::Text &letter, TileMap::TileProperty prop);

	static sf::Vector2u s_maxWindowSize;
public:
	MapMaker(std::string tileSetName, sf::Vector2u tileSize, unsigned int mapWidth, unsigned int mapHeight);
	void display();
	void save(std::string fileName);
	void load(std::string fileName);
	static sf::Font mainFont;
	~MapMaker();
};
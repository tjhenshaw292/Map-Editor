#pragma once
#include "TileMap.h"

class TaskWindow;
//maybe show selected tile

enum Direction
{
	UP, DOWN, LEFT, RIGHT
};

class MovableScreen
{
	sf::View m_view;
	sf::Vector2u m_size;
	sf::Vector2f m_speed;
public:
	MovableScreen(sf::Vector2u &screenSize);
	void assignTileMap(TileMap &map);
	bool canMove(Direction);
	void move(Direction, float distance);
	void move(sf::Vector2f &distance);
	void zoom(float amount);
	const sf::View& getView();
	void setViewport(sf::FloatRect &port);
	sf::Vector2f getPixelRatio();
	void setSpeed(float x, float y);
	const sf::Vector2f& getSpeed();
	sf::Vector2f getRelativeSpeed();
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
	std::string m_mapName;

	void createLines();
	void createLetters();
	void handleClick(sf::Mouse::Button button, sf::Vector2i &position);
	void handleKeyPress(sf::Event &eventy);
	void handleKeyHold(sf::Keyboard::Key key, sf::Vector2i &position);
	//Mouse X Position Relative to Window
	void handleMovementKeys(int xMousePosition);
	void handleMouseScroll(sf::Event &eventy);
	void setProperty(sf::Text &letter, TileMap::TileProperty prop);
	void updateScreens(sf::Time);
	void updateMouseTile();

	static sf::Vector2u s_maxWindowSize;
	static float s_screenSpeed;
	sf::Clock m_clock;
public:
	MapMaker(std::string tileSetName, sf::Vector2u tileSize, unsigned int xTiles, unsigned int yTiles);
	void setBlankTile(int tileNumber);
	void setMapFileName(std::string name);
	void display();
	void save(std::string fileName);
	void load(std::string fileName);
	static sf::Font mainFont;
	~MapMaker();
};
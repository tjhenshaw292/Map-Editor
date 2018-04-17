#include "stdafx.h"
#include "MapMaker.h"
#include "TaskBar.h"

MapMaker::MapMaker(std::string tileSetName, sf::Vector2u tileSize, unsigned int mapWidth, unsigned int mapHeight): 
	m_tileSetName{ tileSetName }, 
	m_tileSize{ tileSize }, 
	m_selectedTile{ 0 },
	m_mapScreen{ sf::Vector2u(s_maxWindowSize.x / 2, s_maxWindowSize.y) },
	m_tileScreen{ sf::Vector2u(s_maxWindowSize.x / 2, s_maxWindowSize.y) }
{
	if (!m_tileSet.loadFromFile(m_tileSetName))
		throw;

	//initialize tile objects
	m_background.create(&m_tileSet, sf::Vector2u(mapWidth, mapHeight), m_tileSize);
	m_foreground.create(&m_tileSet, sf::Vector2u(mapWidth, mapHeight), m_tileSize);
	m_mask.create(&m_tileSet, sf::Vector2u(mapWidth, mapHeight), m_tileSize);
	m_tileSheet.create(&m_tileSet, m_tileSet.getSize(), m_tileSize);

	for (unsigned int i{ 0 }; i < m_tileSheet.m_tiles.size(); ++i)
		m_tileSheet.m_tiles[i].tileNumber = i;

	for (auto &element : m_foreground.m_tiles)
		element.tileNumber = 550;
	for (auto &element : m_mask.m_tiles)
		element.tileNumber = 550;

	m_background.reAssign(m_tileSize);
	m_foreground.reAssign(m_tileSize);
	m_tileSheet.reAssign(m_tileSize);
	m_mask.reAssign(m_tileSize);

	m_windowHeight = m_background.getSize().y >= m_tileSheet.getSize().y ? m_background.getSize().y : m_tileSheet.getSize().y; //get bigger height
	m_tileSheet.setPosition(static_cast<float>(m_background.getSize().x), 0);
	createLines();
	m_background.sectionTiles(m_tileSize);
	m_tileSheet.sectionTiles(m_tileSize);
	m_mask.sectionTiles(m_tileSize);
	createLetters();
}

void MapMaker::display()
{
	unsigned int windowWidth{ m_background.getSize().x + m_tileSheet.getSize().x };

	

	sf::Vector2f mapRatioToTileset;

	//Caps max window Size
	/*if (m_windowHeight > s_maxWindowSize.y)
	{
		m_windowHeight = s_maxWindowSize.y;
	}

	if (windowWidth > s_maxWindowSize.x)
	{
		windowWidth = s_maxWindowSize.x;
		/*if (m_tileSheet.getSize().x >= 500 && m_background.getSize().x >= 500)
		{
			mapRatioToTileset.x = 0.5f;
		}
		else if (m_tileSheet.getSize().x >= 500 && m_background.getSize().x < 500)
		{
			float difference{ 500.0f - m_background.getSize().x };
			mapRatioToTileset.x = (500.0f + difference) / 1000.0f;
		}
		else
		{
			float difference{ 500.0f - m_tileSheet.getSize().x };
			mapRatioToTileset.x = (500.0f - difference) / 1000.0f;
		}
	}*/


	//Handle ViewPorts
	m_mapViewport.left = 0.0f;
	m_mapViewport.top = 0.0f;
	//m_mapViewport.width = mapRatioToTileset.x;
	m_mapViewport.width = 0.5f;
	m_mapViewport.height = 1.0f;
	//m_tileViewport.left = mapRatioToTileset.x;
	m_tileViewport.left = 0.5f;
	m_tileViewport.top = 0.0f;
	//m_tileViewport.width = (1.0f - mapRatioToTileset.x);
	m_tileViewport.width = 0.5f;
	m_tileViewport.height = 1.0f;

	m_mapScreen.assignTileMap(m_background);
	m_tileScreen.assignTileMap(m_tileSheet);
	m_mapScreen.setViewport(m_mapViewport);
	m_tileScreen.setViewport(m_tileViewport);


	//m_window.create(sf::VideoMode(windowWidth, m_windowHeight), "Map Editor");
	m_window.create(sf::VideoMode(s_maxWindowSize.x, s_maxWindowSize.y), "Map Editor");
	m_window.setKeyRepeatEnabled(false);

	//Make Task Window
	sf::Vector2u taskWindowSize{ 300, 300 };
	m_taskWindow = new TaskWindow{ taskWindowSize, &m_tileSet, m_tileSize, mainFont };
	m_taskWindow->setCurrentTile(m_selectedTile);
	m_taskWindow->m_window.setKeyRepeatEnabled(false);
	m_taskWindow->m_window.setPosition(sf::Vector2i{ 20, 73 });

	//Move Window
	m_window.setPosition(sf::Vector2i{ static_cast<int>(taskWindowSize.x) + 40, 73 });
	
	sf::Event eventy;

	while (m_window.isOpen())
	{
		//Main Window Event Loop
		while (m_window.pollEvent(eventy))
		{
			if (eventy.type == sf::Event::KeyPressed)
				handleKeyPress(eventy);
			if (eventy.type == sf::Event::MouseWheelScrolled)
				handleMouseScroll(eventy);
			if (eventy.type == sf::Event::Resized)
				m_window.setSize(sf::Vector2u(windowWidth, m_windowHeight));
			if (eventy.type == sf::Event::Closed)
				m_window.close();
		}

		//Main Window Active
		if (m_window.hasFocus())
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				handleClick(sf::Mouse::Left, sf::Mouse::getPosition(m_window));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				handleClick(sf::Mouse::Right, sf::Mouse::getPosition(m_window));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
				handleClick(sf::Mouse::Middle, sf::Mouse::getPosition(m_window));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
				handleKeyHold(sf::Keyboard::B, sf::Mouse::getPosition(m_window));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				handleKeyHold(sf::Keyboard::S, sf::Mouse::getPosition(m_window));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				handleKeyHold(sf::Keyboard::A, sf::Mouse::getPosition(m_window));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				handleKeyHold(sf::Keyboard::W, sf::Mouse::getPosition(m_window));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
				handleKeyHold(sf::Keyboard::N, sf::Mouse::getPosition(m_window));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				handleKeyHold(sf::Keyboard::D, sf::Mouse::getPosition(m_window));
		}

		//Task Window Event Loop
		while (m_taskWindow->m_window.pollEvent(eventy))
		{
			if (eventy.type == sf::Event::KeyPressed)
				handleKeyPress(eventy);
			if (eventy.type == sf::Event::MouseButtonPressed)
				m_taskWindow->handleClick(sf::Mouse::getPosition(m_taskWindow->m_window));
			if (eventy.type == sf::Event::Resized)
				m_taskWindow->m_window.setSize(m_taskWindow->m_size);
			//if (eventy.type == sf::Event::Closed)
				//m_taskWindow->m_window.close();

			//Gets Focus to handle key press events
			m_window.requestFocus();
		}
		
		//Draw Loop
		m_window.clear(sf::Color::Black);

		//Map Screen
		m_window.setView(m_mapScreen.getView());
		if (m_taskWindow->m_BGToggle.isOn())
			m_window.draw(m_background);
		if (m_taskWindow->m_FGToggle.isOn())
			m_window.draw(m_foreground);
		if (m_taskWindow->m_maskToggle.isOn())
			m_window.draw(m_mask);
		if (m_taskWindow->m_lineToggle.isOn())
			m_window.draw(m_mapLines);
		if (m_taskWindow->m_properties.isOn())
		{
			for (auto &element : m_letters)
			{
				m_window.draw(element);
			}
		}

		//Tile Screen
		m_window.setView(m_tileScreen.getView());
		m_window.draw(m_tileSheet);
		if (m_taskWindow->m_lineToggle.isOn())
			m_window.draw(m_tileLines);

		//Display Windows
		m_window.display();
		m_taskWindow->display();
	}
}

void MapMaker::createLines()
{
	unsigned int horizMapVertices{ (m_background.getTileAmount().y - 1) * 2 };
	unsigned int vertMapVertices{ (m_background.getTileAmount().x - 1) * 2 };
	unsigned int horizSheetVertices{ (m_tileSheet.getTileAmount().y - 1) * 2 };
	unsigned int vertSheetVertices{ (m_tileSheet.getTileAmount().x - 1) * 2 };

	m_mapLines.setPrimitiveType(sf::Lines);
	m_mapLines.resize(horizMapVertices + vertMapVertices + 8); //8 blue vertices
	m_tileLines.setPrimitiveType(sf::Lines);
	m_tileLines.resize(horizSheetVertices + vertSheetVertices + 8); //8 blue vertices

	int bounds[4];
	bounds[0] = static_cast<int>(horizMapVertices);
	bounds[1] = bounds[0] + static_cast<int>(vertMapVertices);

	bounds[2] = static_cast<int>(horizSheetVertices);
	bounds[3] = bounds[2] + static_cast<int>(vertSheetVertices);

	//bounds[2] = bounds[1] + static_cast<int>(horizSheetVertices);
	//bounds[3] = bounds[2] + static_cast<int>(vertSheetVertices);

	unsigned int tileSheetXOffset{ m_background.getSize().x };

	//horiz map lines
	for (int i{ 0 }, k{ static_cast<int>(m_tileSize.y) }; i < bounds[0]; i += 2, k += static_cast<int>(m_tileSize.y))
	{
		m_mapLines[i].position = sf::Vector2f(0, static_cast<float>(k));
		m_mapLines[i + 1].position = sf::Vector2f(static_cast<float>(tileSheetXOffset), static_cast<float>(k));
		m_mapLines[i].color = sf::Color::Red;
		m_mapLines[i + 1].color = sf::Color::Red;
	}
	//vertical map lines
	for (int i{ bounds[0] }, k{ static_cast<int>(m_tileSize.x) }; i < bounds[1]; i += 2, k += static_cast<int>(m_tileSize.x))
	{
		m_mapLines[i].position = sf::Vector2f(static_cast<float>(k), 0);
		m_mapLines[i + 1].position = sf::Vector2f(static_cast<float>(k), static_cast<float>(m_background.getSize().y));
		m_mapLines[i].color = sf::Color::Red;
		m_mapLines[i + 1].color = sf::Color::Red;
	}
	//horiz sheet lines
	//for (int i{ bounds[1] }, k{ static_cast<int>(m_tileSize.y) }; i < bounds[2]; i += 2, k += static_cast<int>(m_tileSize.y))
	for (int i{ 0 }, k{ static_cast<int>(m_tileSize.y) }; i < bounds[2]; i += 2, k += static_cast<int>(m_tileSize.y))
	{
		m_tileLines[i].position = sf::Vector2f(static_cast<float>(tileSheetXOffset), static_cast<float>(k));
		m_tileLines[i + 1].position = sf::Vector2f(static_cast<float>(tileSheetXOffset + m_tileSheet.getSize().x), static_cast<float>(k));
		m_tileLines[i].color = sf::Color::Red;
		m_tileLines[i + 1].color = sf::Color::Red;
	}
	//vertical sheet lines
	for (int i{ bounds[2] }, k{ static_cast<int>(m_tileSize.x) }; i < bounds[3]; i += 2, k += static_cast<int>(m_tileSize.x))
	{
		m_tileLines[i].position = sf::Vector2f(static_cast<float>(k + tileSheetXOffset), 0);
		m_tileLines[i + 1].position = sf::Vector2f(static_cast<float>(k + tileSheetXOffset), static_cast<float>(m_tileSheet.getSize().y));
		m_tileLines[i].color = sf::Color::Red;
		m_tileLines[i + 1].color = sf::Color::Red;
	}
	//blue Lines
	/*m_lines[bounds[3]].position = sf::Vector2f(0, static_cast<float>(m_background.getSize().y));
	m_lines[bounds[3] + 1].position = sf::Vector2f(static_cast<float>(tileSheetXOffset), static_cast<float>(m_background.getSize().y));
	m_lines[bounds[3] + 2].position = sf::Vector2f(static_cast<float>(tileSheetXOffset), 0);
	m_lines[bounds[3] + 3].position = sf::Vector2f(static_cast<float>(tileSheetXOffset), static_cast<float>(m_windowHeight));
	m_lines[bounds[3] + 4].position = sf::Vector2f(static_cast<float>(tileSheetXOffset), static_cast<float>(m_tileSheet.getSize().y));
	m_lines[bounds[3] + 5].position = sf::Vector2f(static_cast<float>(tileSheetXOffset + m_tileSheet.getSize().x), static_cast<float>(m_tileSheet.getSize().y));
	for (int i{ bounds[3] }; i < bounds[3] + 6; i++)
		m_lines[i].color = sf::Color::Blue;*/
}

void MapMaker::createLetters()
{
	sf::Text letter;
	letter.setFont(mainFont);
	letter.setStyle(sf::Text::Bold);
	letter.setCharacterSize(14);
	
	for (auto &element : m_background.m_tiles)
	{
		setProperty(letter, element.tileProperty);
		letter.setPosition(element.tileRect.left + element.tileRect.width / 2, element.tileRect.top + element.tileRect.height / 4);
		m_letters.push_back(sf::Text(letter));
	}
}

void MapMaker::setProperty(sf::Text &letter, TileMap::TileProperty prop)
{
	switch (prop)
	{
	case TileMap::BLOCKED:
		letter.setString('B');
		letter.setFillColor(sf::Color::Red);
		break;
	case TileMap::NOTHING:
		letter.setString(' ');
		break;
	case TileMap::ACTION:
		letter.setString('A');
		letter.setFillColor(sf::Color::Green);
		break;
	case TileMap::SPECIAL:
		letter.setString('S');
		letter.setFillColor(sf::Color::Magenta);
		break;
	case TileMap::WARP:
		letter.setString('W');
		letter.setFillColor(sf::Color::Blue);
		break;
	case TileMap::DOOR:
		letter.setString('D');
		letter.setFillColor(sf::Color::Blue);
		break;
	}
	letter.setOrigin(sf::Vector2f(letter.getGlobalBounds().width / 2, letter.getGlobalBounds().height / 2));
}

void MapMaker::handleClick(sf::Mouse::Button button, sf::Vector2i &position)
{
	bool onMap;

	//Set view to appropriate screen
	if (position.x < static_cast<int>(s_maxWindowSize.x) / 2)
	{
		m_window.setView(m_mapScreen.getView());
		onMap = true;
	}
	else
	{
		m_window.setView(m_tileScreen.getView());
		onMap = false;
	}

	sf::Vector2f actualPosition{ m_window.mapPixelToCoords(position) };

	switch (button)
	{
	case sf::Mouse::Left:
		if (m_tileSheet.contains(actualPosition) && !onMap)
		{
			m_selectedTile = m_tileSheet.getTileNumber(actualPosition);
			m_taskWindow->setCurrentTile(m_selectedTile);
		}
		else if (m_background.contains(actualPosition) && onMap)
		{
			m_background.m_tiles[m_background.getTileIndex(actualPosition)].tileNumber = m_selectedTile; //the jesus line
			m_background.reAssign(m_tileSize);
		}
		break;
	case sf::Mouse::Right:
		if (m_background.contains(actualPosition) && onMap)
		{
			m_foreground.m_tiles[m_background.getTileIndex(actualPosition)].tileNumber = m_selectedTile;
			m_foreground.reAssign(m_tileSize);
		}
		break;
	case sf::Mouse::Middle:
		if (m_background.contains(actualPosition) && onMap)
		{
			m_mask.m_tiles[m_background.getTileIndex(actualPosition)].tileNumber = m_selectedTile;
			m_mask.reAssign(m_tileSize);
		}
		break;
	}
}

void MapMaker::handleKeyHold(sf::Keyboard::Key key, sf::Vector2i &position)
{
	//Set view to appropriate screen
	if (position.x < static_cast<int>(s_maxWindowSize.x) / 2)
		m_window.setView(m_mapScreen.getView());
	else
		return; //no key hold on tilesheet

	sf::Vector2f actualPosition{ m_window.mapPixelToCoords(position) };

	switch (key)
	{
	case sf::Keyboard::B:
		if (m_background.contains(actualPosition))
		{
			m_background.m_tiles[m_background.getTileIndex(actualPosition)].tileProperty = TileMap::BLOCKED;
			setProperty(m_letters[m_background.getTileIndex(actualPosition)], TileMap::BLOCKED);
		}
		break;
	case sf::Keyboard::W:
		if (m_background.contains(actualPosition))
		{
			m_background.m_tiles[m_background.getTileIndex(actualPosition)].tileProperty = TileMap::WARP;
			setProperty(m_letters[m_background.getTileIndex(actualPosition)], TileMap::WARP);
		}
		break;
	case sf::Keyboard::S:
		if (m_background.contains(actualPosition))
		{
			m_background.m_tiles[m_background.getTileIndex(actualPosition)].tileProperty = TileMap::SPECIAL;
			setProperty(m_letters[m_background.getTileIndex(actualPosition)], TileMap::SPECIAL);
		}
		break;
	case sf::Keyboard::A:
		if (m_background.contains(actualPosition))
		{
			m_background.m_tiles[m_background.getTileIndex(actualPosition)].tileProperty = TileMap::ACTION;
			setProperty(m_letters[m_background.getTileIndex(actualPosition)], TileMap::ACTION);
		}
		break;
	case sf::Keyboard::N:
		if (m_background.contains(actualPosition))
		{
			m_background.m_tiles[m_background.getTileIndex(actualPosition)].tileProperty = TileMap::NOTHING;
			setProperty(m_letters[m_background.getTileIndex(actualPosition)], TileMap::NOTHING);
		}
		break;
	case sf::Keyboard::D:
		if (m_background.contains(actualPosition))
		{
			m_background.m_tiles[m_background.getTileIndex(actualPosition)].tileProperty = TileMap::DOOR;
			setProperty(m_letters[m_background.getTileIndex(actualPosition)], TileMap::DOOR);
		}
		break;
	case sf::Keyboard::Up:
		m_mapScreen.move(UP);
		break;
	case sf::Keyboard::Down:
		m_mapScreen.move(DOWN);
		break;
	}
	std::cout << m_background.getTileIndex(actualPosition) << std::endl;
}

void MapMaker::handleMouseScroll(sf::Event &eventy)
{
	if (eventy.mouseWheelScroll.x < static_cast<int>(s_maxWindowSize.x) / 2)
	{
		m_mapScreen.zoom(eventy.mouseWheelScroll.delta * 0.1f);
	}
	else
	{
		m_tileScreen.zoom(eventy.mouseWheelScroll.delta * 0.1f);
	}
}

void MapMaker::handleKeyPress(sf::Event &eventy)
{
	switch (eventy.key.code)
	{
	case sf::Keyboard::L:
		m_taskWindow->m_lineToggle.toggle();
		break;
	case sf::Keyboard::Num1:
	case sf::Keyboard::G:
		m_taskWindow->m_BGToggle.toggle();
		break;
	case sf::Keyboard::Num2:
	case sf::Keyboard::F:
		m_taskWindow->m_FGToggle.toggle();
		break;
	case sf::Keyboard::Num3:
	case sf::Keyboard::M:
		m_taskWindow->m_maskToggle.toggle();
		break;
	case sf::Keyboard::P:
		m_taskWindow->m_properties.toggle();
		break;
		//Map and Tile Screen Movement
	/*case sf::Keyboard::Up:
		if (sf::Mouse::getPosition(m_window).x < static_cast<int>(s_maxWindowSize.x) / 2)
			m_mapScreen.move(UP, 30.0f);
		else
			m_tileScreen.move(UP, 30.0f);
		break;
	case sf::Keyboard::Down:
		if (sf::Mouse::getPosition(m_window).x < static_cast<int>(s_maxWindowSize.x) / 2)
			m_mapScreen.move(DOWN, 30.0f);
		else
			m_tileScreen.move(DOWN, 30.0f);
		break;*/
	case sf::Keyboard::Left:
		if (sf::Mouse::getPosition(m_window).x < static_cast<int>(s_maxWindowSize.x) / 2)
			m_mapScreen.move(LEFT, 30.0f);
		else
			m_tileScreen.move(LEFT, 30.0f);
		break;
	case sf::Keyboard::Right:
		if (sf::Mouse::getPosition(m_window).x < static_cast<int>(s_maxWindowSize.x) / 2)
			m_mapScreen.move(RIGHT, 30.0f);
		else
			m_tileScreen.move(RIGHT, 30.0f);
		break;
	}
}

void MapMaker::save(std::string fileName)
{
	std::ofstream output(fileName);
	if (!output)
		throw;
	output << m_background.getTileAmount().x << " "; //saves tile amount
	output << m_background.getTileAmount().y << " ";
	for (auto &element : m_background.m_tiles)
		output << element.tileNumber << " ";
	for (auto &element : m_foreground.m_tiles)
		output << element.tileNumber << " ";
	for (auto &element : m_mask.m_tiles)
		output << element.tileNumber << " ";
	for (auto &element : m_background.m_tiles)
		output << element.tileProperty << " ";
	output.close();
}

void MapMaker::load(std::string fileName)
{
	std::ifstream input(fileName);
	if (!input)
		throw;
	int trash;
	input >> trash; //throw away first two values for tile amount
	input >> trash;

	for (auto &element : m_background.m_tiles)
		input >> element.tileNumber;

	for (auto &element : m_foreground.m_tiles)
		input >> element.tileNumber;

	for (auto &element : m_mask.m_tiles)
		input >> element.tileNumber;

	int temp;
	int i{ 0 };
	for (auto &element : m_background.m_tiles)
	{
		input >> temp;
		element.tileProperty = static_cast<TileMap::TileProperty>(temp);
		setProperty(m_letters[i++], element.tileProperty);
	}

	m_background.reAssign(m_tileSize);
	m_foreground.reAssign(m_tileSize);
	m_mask.reAssign(m_tileSize);

	input.close();
}

MapMaker::~MapMaker()
{
	delete m_taskWindow;
}

MovableScreen::MovableScreen(sf::Vector2u &screenSize) :
	m_size{ screenSize }
{

}

void MovableScreen::assignTileMap(TileMap &map)
{
	sf::Vector2f mapSize{ static_cast<float>(map.getSize().x), static_cast<float>(map.getSize().y) };
	sf::Vector2f screenSize{ static_cast<float>(m_size.x), static_cast<float>(m_size.y) };

	sf::Vector2f aspectRatio{ screenSize.y / screenSize.x, screenSize.x / screenSize.y };

	if (mapSize.x * aspectRatio.x >= mapSize.y * aspectRatio.y)
	{
		m_view.setSize(mapSize.x, mapSize.x * aspectRatio.x);
		m_view.setCenter(mapSize.x / 2.0f + map.getPosition().x, mapSize.x * aspectRatio.x / 2.0f + map.getPosition().y);
	}
	else
	{
		m_view.setSize(mapSize.y * aspectRatio.y, mapSize.y);
		m_view.setCenter(mapSize.y * aspectRatio.y / 2.0f + map.getPosition().x, mapSize.y / 2.0f + map.getPosition().y);
	}
}

bool MovableScreen::canMove(Direction moving)
{
	switch (moving)
	{
	case UP:
		//if (m_view.getCenter)
		break;
	case DOWN:
		break;
	case LEFT:
		break;
	case RIGHT:
		break;
	}
	return true;
}

void MovableScreen::move(Direction moving)
{
	switch (moving)
	{
	case UP:
		m_view.move(0, -amount);
		break;
	case DOWN:
		m_view.move(0, amount);
		break;
	case LEFT:
		m_view.move(-amount, 0);
		break;
	case RIGHT:
		m_view.move(amount, 0);
		break;
	default:
		throw; //wrong input
	}
}

void MovableScreen::move(Direction moving, float amount)
{
	switch (moving)
	{
	case UP:
		m_view.move(0, -amount);
		break;
	case DOWN:
		m_view.move(0, amount);
		break;
	case LEFT:
		m_view.move(-amount, 0);
		break;
	case RIGHT:
		m_view.move(amount, 0);
		break;
	default:
		throw; //wrong input
	}
}

void MovableScreen::zoom(float amount)
{
	m_view.zoom(1 - amount);
}

const sf::View& MovableScreen::getView()
{
	return m_view;
}

void MovableScreen::setViewport(sf::FloatRect &port)
{
	m_view.setViewport(port);
}

sf::Font MapMaker::mainFont;
sf::Vector2u MapMaker::s_maxWindowSize{ 1300, 900 }; //gotta be 2:1 aspect ratio
float MovableScreen::s_screenSpeed{ 30.0f };
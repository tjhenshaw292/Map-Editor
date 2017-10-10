#include "stdafx.h"
#include "MapMaker.h"

MapMaker::MapMaker(std::string tileSetName, sf::Vector2u tileSize, unsigned int mapWidth, unsigned int mapHeight)
	: m_tileSetName{ tileSetName }, m_tileSize{ tileSize }, m_selectedTile{ 0 }, drawLines{ true }, drawBackground{ true }, drawForeground{ true }, drawLetters{ true }, drawMask{ true }
{
	if (!m_tileSet.loadFromFile(m_tileSetName))
		throw;
	
	if (!m_font.loadFromFile("Carlito-Regular.ttf"))
		throw;

	//initialize tile objects
	m_background.create(&m_tileSet, sf::Vector2u(mapWidth, mapHeight), m_tileSize);
	m_foreground.create(&m_tileSet, sf::Vector2u(mapWidth, mapHeight), m_tileSize);
	m_mask.create(&m_tileSet, sf::Vector2u(mapWidth, mapHeight), m_tileSize);
	m_tileSheet.create(&m_tileSet, m_tileSet.getSize(), m_tileSize);

	for (int i{ 0 }; i < m_tileSheet.m_tiles.size(); ++i)
		m_tileSheet.m_tiles[i].tileNumber = i;

	for (auto &element : m_foreground.m_tiles)
		element.tileNumber = 550;
	for (auto &element : m_mask.m_tiles)
		element.tileNumber = 550;

	m_background.reAssign(m_tileSize);
	m_foreground.reAssign(m_tileSize);
	m_tileSheet.reAssign(m_tileSize);
	m_mask.reAssign(m_tileSize);

	m_background.getSize().y >= m_tileSheet.getSize().y ? m_windowHeight = m_background.getSize().y : m_windowHeight = m_tileSheet.getSize().y; //get bigger height
	m_tileSheet.setPosition(m_background.getSize().x, 0);
	createLines();
	m_background.sectionTiles(m_tileSize);
	m_tileSheet.sectionTiles(m_tileSize);
	m_mask.sectionTiles(m_tileSize);
	createLetters();
}

void MapMaker::display()
{
	unsigned int windowWidth{ m_background.getSize().x + m_tileSheet.getSize().x };
	m_window.create(sf::VideoMode(windowWidth, m_windowHeight), "Map Editor");

	sf::Event eventy;

	while (m_window.isOpen())
	{
		m_window.clear(sf::Color::Black);
		m_window.draw(m_tileSheet);
		if (drawBackground)
			m_window.draw(m_background);
		if (drawForeground)
			m_window.draw(m_foreground);
		if (drawMask)
			m_window.draw(m_mask);
		if (drawLines)
			m_window.draw(m_lines);
		if (drawLetters)
			for (auto &element : m_letters)
				m_window.draw(element);
		m_window.display();
		m_window.pollEvent(eventy);
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
		if (eventy.type == sf::Event::KeyPressed)
			handleKeyPress(eventy);
		if (eventy.type == sf::Event::Resized)
			m_window.setSize(sf::Vector2u(windowWidth, m_windowHeight));
		if (eventy.type == sf::Event::Closed)
			m_window.close();
	}
}

void MapMaker::createLines()
{
	unsigned int horizMapVertices{ (m_background.getTileAmount().y - 1) * 2 };
	unsigned int vertMapVertices{ (m_background.getTileAmount().x - 1) * 2 };
	unsigned int horizSheetVertices{ (m_tileSheet.getTileAmount().y - 1) * 2 };
	unsigned int vertSheetVertices{ (m_tileSheet.getTileAmount().x - 1) * 2 };

	m_lines.setPrimitiveType(sf::Lines);
	m_lines.resize(horizMapVertices + vertMapVertices + horizSheetVertices + vertSheetVertices + 6); //6 blue vertices

	int bounds[4];
	bounds[0] = static_cast<int>(horizMapVertices);
	bounds[1] = bounds[0] + static_cast<int>(vertMapVertices);
	bounds[2] = bounds[1] + static_cast<int>(horizSheetVertices);
	bounds[3] = bounds[2] + static_cast<int>(vertSheetVertices);

	unsigned int tileSheetXOffset{ m_background.getSize().x };

	//horiz map lines
	for (int i{ 0 }, k{ static_cast<int>(m_tileSize.y) }; i < bounds[0]; i += 2, k += static_cast<int>(m_tileSize.y))
	{
		m_lines[i].position = sf::Vector2f(0, k);
		m_lines[i + 1].position = sf::Vector2f(tileSheetXOffset, k);
		m_lines[i].color = sf::Color::Red;
		m_lines[i + 1].color = sf::Color::Red;
	}
	//vertical map lines
	for (int i{ bounds[0] }, k{ static_cast<int>(m_tileSize.x) }; i < bounds[1]; i += 2, k += static_cast<int>(m_tileSize.x))
	{
		m_lines[i].position = sf::Vector2f(k, 0);
		m_lines[i + 1].position = sf::Vector2f(k, m_background.getSize().y);
		m_lines[i].color = sf::Color::Red;
		m_lines[i + 1].color = sf::Color::Red;
	}
	//horiz sheet lines
	for (int i{ bounds[1] }, k{ static_cast<int>(m_tileSize.y) }; i < bounds[2]; i += 2, k += static_cast<int>(m_tileSize.y))
	{
		m_lines[i].position = sf::Vector2f(tileSheetXOffset, k);
		m_lines[i + 1].position = sf::Vector2f(tileSheetXOffset + m_tileSheet.getSize().x, k);
		m_lines[i].color = sf::Color::Red;
		m_lines[i + 1].color = sf::Color::Red;
	}
	//vertical sheet lines
	for (int i{ bounds[2] }, k{ static_cast<int>(m_tileSize.x) }; i < bounds[3]; i += 2, k += static_cast<int>(m_tileSize.x))
	{
		m_lines[i].position = sf::Vector2f(k + tileSheetXOffset, 0);
		m_lines[i + 1].position = sf::Vector2f(k + tileSheetXOffset, m_tileSheet.getSize().y);
		m_lines[i].color = sf::Color::Red;
		m_lines[i + 1].color = sf::Color::Red;
	}
	//blue Lines
	m_lines[bounds[3]].position = sf::Vector2f(0, m_background.getSize().y);
	m_lines[bounds[3] + 1].position = sf::Vector2f(tileSheetXOffset, m_background.getSize().y);
	m_lines[bounds[3] + 2].position = sf::Vector2f(tileSheetXOffset, 0);
	m_lines[bounds[3] + 3].position = sf::Vector2f(tileSheetXOffset, m_windowHeight);
	m_lines[bounds[3] + 4].position = sf::Vector2f(tileSheetXOffset, m_tileSheet.getSize().y);
	m_lines[bounds[3] + 5].position = sf::Vector2f(tileSheetXOffset + m_tileSheet.getSize().x, m_tileSheet.getSize().y);
	for (int i{ bounds[3] }; i < bounds[3] + 6; i++)
		m_lines[i].color = sf::Color::Blue;
}

void MapMaker::createLetters()
{
	sf::Text letter;
	letter.setFont(m_font);
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
		letter.setString('\0');
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
	switch (button)
	{
	case sf::Mouse::Left:
		if (m_tileSheet.contains(position))
		{
			m_selectedTile = m_tileSheet.getTileNumber(position);
		}
		else if (m_background.contains(position))
		{
			m_background.m_tiles[m_background.getTileIndex(position)].tileNumber = m_selectedTile; //the jesus line
			m_background.reAssign(m_tileSize);
		}
		break;
	case sf::Mouse::Right:
		if (m_background.contains(position))
		{
			m_foreground.m_tiles[m_background.getTileIndex(position)].tileNumber = m_selectedTile;
			m_foreground.reAssign(m_tileSize);
		}
		break;
	case sf::Mouse::Middle:
		if (m_background.contains(position))
		{
			m_mask.m_tiles[m_background.getTileIndex(position)].tileNumber = m_selectedTile;
			m_mask.reAssign(m_tileSize);
		}
		break;
	}
}

void MapMaker::handleKeyHold(sf::Keyboard::Key key, sf::Vector2i &position)
{
	switch (key)
	{
	case sf::Keyboard::B:
		if (m_background.contains(position))
		{
			m_background.m_tiles[m_background.getTileIndex(position)].tileProperty = TileMap::BLOCKED;
			setProperty(m_letters[m_background.getTileIndex(position)], TileMap::BLOCKED);
		}
		break;
	case sf::Keyboard::W:
		if (m_background.contains(position))
		{
			m_background.m_tiles[m_background.getTileIndex(position)].tileProperty = TileMap::WARP;
			setProperty(m_letters[m_background.getTileIndex(position)], TileMap::WARP);
		}
		break;
	case sf::Keyboard::S:
		if (m_background.contains(position))
		{
			m_background.m_tiles[m_background.getTileIndex(position)].tileProperty = TileMap::SPECIAL;
			setProperty(m_letters[m_background.getTileIndex(position)], TileMap::SPECIAL);
		}
		break;
	case sf::Keyboard::A:
		if (m_background.contains(position))
		{
			m_background.m_tiles[m_background.getTileIndex(position)].tileProperty = TileMap::ACTION;
			setProperty(m_letters[m_background.getTileIndex(position)], TileMap::ACTION);
		}
		break;
	case sf::Keyboard::N:
		if (m_background.contains(position))
		{
			m_background.m_tiles[m_background.getTileIndex(position)].tileProperty = TileMap::NOTHING;
			setProperty(m_letters[m_background.getTileIndex(position)], TileMap::NOTHING);
		}
		break;
	case sf::Keyboard::D:
		if (m_background.contains(position))
		{
			m_background.m_tiles[m_background.getTileIndex(position)].tileProperty = TileMap::DOOR;
			setProperty(m_letters[m_background.getTileIndex(position)], TileMap::DOOR);
		}
		break;
	}
	std::cout << m_background.getTileIndex(position) << std::endl;
}

void MapMaker::handleKeyPress(sf::Event &eventy)
{
	if (eventy.key.code == sf::Keyboard::L)
	{
		if (drawLines)
			drawLines = false;
		else
			drawLines = true;
	}
	if (eventy.key.code == sf::Keyboard::Num2 || eventy.key.code == sf::Keyboard::F)
	{
		if (drawForeground)
			drawForeground = false;
		else
			drawForeground = true;
	}
	if (eventy.key.code == sf::Keyboard::Num1 || eventy.key.code == sf::Keyboard::G)
	{
		if (drawBackground)
			drawBackground = false;
		else
			drawBackground = true;
	}
	if (eventy.key.code == sf::Keyboard::Num3 || eventy.key.code == sf::Keyboard::M)
	{
		if (drawMask)
			drawMask = false;
		else
			drawMask = true;
	}
	if (eventy.key.code == sf::Keyboard::P)
	{
		if (drawLetters)
			drawLetters = false;
		else
			drawLetters = true;
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
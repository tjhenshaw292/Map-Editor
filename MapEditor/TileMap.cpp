#include "stdafx.h"
#include "TileMap.h"

void TileMap::create(sf::Texture *tileSet, sf::Vector2u &size, sf::Vector2u &tileSize)
{
	m_tileSet = tileSet;
	m_size = size;

	m_tileAmount.x = m_size.x / tileSize.x;
	m_tileAmount.y = m_size.y / tileSize.y;

	//populate tile vector with zeroes
	m_tiles.resize(m_tileAmount.x * m_tileAmount.y);
	for (auto &element : m_tiles)
	{
		element.tileNumber = 0;
		element.tileProperty = NOTHING;
	}

	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(m_tileAmount.x * m_tileAmount.y * 4);
}

void TileMap::reAssign(sf::Vector2u &tileSize)
{
	// populate the vertex array, with one quad per tile
	for (unsigned int i = 0; i < m_tileAmount.x; ++i)
		for (unsigned int j = 0; j < m_tileAmount.y; ++j)
		{
			// get the current tile number
			int tileNumber = m_tiles[i + j * m_tileAmount.x].tileNumber;

			// find its position in the tileset texture
			int tu = tileNumber % (m_tileSet->getSize().x / tileSize.x);
			int tv = tileNumber / (m_tileSet->getSize().x / tileSize.x);

			// get a pointer to the current tile's quad
			sf::Vertex* quad = &m_vertices[(i + j * m_tileAmount.x) * 4];

			// define its 4 corners
			quad[0].position = sf::Vector2f(static_cast<float>(i * tileSize.x), static_cast<float>(j * tileSize.y));
			quad[1].position = sf::Vector2f(static_cast<float>((i + 1) * tileSize.x), static_cast<float>(j * tileSize.y));
			quad[2].position = sf::Vector2f(static_cast<float>((i + 1) * tileSize.x), static_cast<float>((j + 1) * tileSize.y));
			quad[3].position = sf::Vector2f(static_cast<float>(i * tileSize.x), static_cast<float>((j + 1) * tileSize.y));

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(static_cast<float>(tu * tileSize.x), static_cast<float>(tv * tileSize.y));
			quad[1].texCoords = sf::Vector2f(static_cast<float>((tu + 1) * tileSize.x), static_cast<float>(tv * tileSize.y));
			quad[2].texCoords = sf::Vector2f(static_cast<float>((tu + 1) * tileSize.x), static_cast<float>((tv + 1) * tileSize.y));
			quad[3].texCoords = sf::Vector2f(static_cast<float>(tu * tileSize.x), static_cast<float>((tv + 1) * tileSize.y));
		}

}

void TileMap::sectionTiles(sf::Vector2u &tileSize)
{
	m_mapRect.top = this->getPosition().y;
	m_mapRect.left = this->getPosition().x;
	m_mapRect.width = static_cast<float>(m_size.x);
	m_mapRect.height = static_cast<float>(m_size.y);
	for (unsigned int i = 0; i < m_tileAmount.x; ++i)
		for (unsigned int j = 0; j < m_tileAmount.y; ++j)
		{
			m_tiles[i + j * m_tileAmount.x].tileRect.top = static_cast<float>(j * tileSize.y);
			m_tiles[i + j * m_tileAmount.x].tileRect.left = static_cast<float>(i * tileSize.x + m_mapRect.left);
			m_tiles[i + j * m_tileAmount.x].tileRect.width = static_cast<float>(tileSize.x);
			m_tiles[i + j * m_tileAmount.x].tileRect.height = static_cast<float>(tileSize.y);
		}
}

int TileMap::getTileNumber(sf::Vector2f &position)
{
	for (auto &element : m_tiles)
	{
		if (element.tileRect.contains(position))
			return element.tileNumber;
	}
	return -1;
}

int TileMap::getTileIndex(sf::Vector2f &position)
{
	for (int i{ 0 }; i < static_cast<int>(m_tiles.size()); i++)
	{
		if (m_tiles[i].tileRect.contains(position))
			return i;
	}
	return -1;
}

void TileMap::addColumnToRight(int tileNumber)
{
	//Get Tile size
	sf::Vector2u tileSize{ m_size.x / m_tileAmount.x, m_size.y / m_tileAmount.y };

	//Add Column
	m_tileAmount.x += 1;

	//Change size
	m_size.x = m_tileAmount.x * tileSize.x;
	m_vertices.resize(m_tileAmount.x * m_tileAmount.y * 4);

	//Create tile to add
	Tile addedTile;
	addedTile.tileNumber = tileNumber;
	addedTile.tileProperty = NOTHING;

	//Insert Column to Tile Vector
	std::vector<Tile> oldTiles{ m_tiles };
	m_tiles.resize(m_tileAmount.x * m_tileAmount.y);
	int skippedTiles{ 0 };
	for (unsigned int row{ 0 }; row < m_tileAmount.y; row++)
	{
		for (unsigned int column{ 0 }; column < m_tileAmount.x; column++)
		{
			//If on last column, add new tile instead of copying old one
			if (column < m_tileAmount.x - 1)
				m_tiles[column + row * m_tileAmount.x] = oldTiles[column + row * m_tileAmount.x - skippedTiles];
			else
			{
				m_tiles[column + row * m_tileAmount.x] = addedTile;
				skippedTiles++;
			}
		}
	}

	sectionTiles(tileSize);
	reAssign(tileSize);
}

void TileMap::addColumnToLeft(int tileNumber)
{
	//Get Tile size
	sf::Vector2u tileSize{ m_size.x / m_tileAmount.x, m_size.y / m_tileAmount.y };

	//Add Column
	m_tileAmount.x += 1;

	//Change size
	m_size.x = m_tileAmount.x * tileSize.x;
	m_vertices.resize(m_tileAmount.x * m_tileAmount.y * 4);

	//Create tile to add
	Tile addedTile;
	addedTile.tileNumber = tileNumber;
	addedTile.tileProperty = NOTHING;

	//Insert Column to Tile Vector
	std::vector<Tile> oldTiles{ m_tiles };
	m_tiles.resize(m_tileAmount.x * m_tileAmount.y);
	int skippedTiles{ 0 };
	for (unsigned int row{ 0 }; row < m_tileAmount.y; row++)
	{
		for (unsigned int column{ 0 }; column < m_tileAmount.x; column++)
		{
			//If on first column, add new tile instead of copying old one
			if (column != 0)
				m_tiles[column + row * m_tileAmount.x] = oldTiles[column + row * m_tileAmount.x - skippedTiles];
			else
			{
				m_tiles[column + row * m_tileAmount.x] = addedTile;
				skippedTiles++;
			}
		}
	}

	sectionTiles(tileSize);
	reAssign(tileSize);
}

void TileMap::addRowToTop(int tileNumber)
{
	//Get Tile size
	sf::Vector2u tileSize{ m_size.x / m_tileAmount.x, m_size.y / m_tileAmount.y };

	//Add Row
	m_tileAmount.y += 1;

	//Change size
	m_size.y = m_tileAmount.y * tileSize.y;
	m_vertices.resize(m_tileAmount.x * m_tileAmount.y * 4);

	//Create tile to add
	Tile addedTile;
	addedTile.tileNumber = tileNumber;
	addedTile.tileProperty = NOTHING;

	//Insert Row to Tile Vector
	std::vector<Tile> oldTiles{ m_tiles };
	m_tiles.resize(m_tileAmount.x * m_tileAmount.y);
	int skippedTiles{ 0 };
	for (unsigned int row{ 0 }; row < m_tileAmount.y; row++)
	{
		for (unsigned int column{ 0 }; column < m_tileAmount.x; column++)
		{
			//If on first row, add new tile instead of copying old one
			if (row != 0)
				m_tiles[column + row * m_tileAmount.x] = oldTiles[column + row * m_tileAmount.x - skippedTiles];
			else
			{
				m_tiles[column + row * m_tileAmount.x] = addedTile;
				skippedTiles++;
			}
		}
	}

	sectionTiles(tileSize);
	reAssign(tileSize);
}

void TileMap::addRowToBottow(int tileNumber)
{
	//Get Tile size
	sf::Vector2u tileSize{ m_size.x / m_tileAmount.x, m_size.y / m_tileAmount.y };

	//Add Row
	m_tileAmount.y += 1;

	//Change size
	m_size.y = m_tileAmount.y * tileSize.y;
	m_vertices.resize(m_tileAmount.x * m_tileAmount.y * 4);

	//Create tile to add
	Tile addedTile;
	addedTile.tileNumber = tileNumber;
	addedTile.tileProperty = NOTHING;

	//Insert Row to Tile Vector
	std::vector<Tile> oldTiles{ m_tiles };
	m_tiles.resize(m_tileAmount.x * m_tileAmount.y);
	int skippedTiles{ 0 };
	for (unsigned int row{ 0 }; row < m_tileAmount.y; row++)
	{
		for (unsigned int column{ 0 }; column < m_tileAmount.x; column++)
		{
			//If on last row, add new tile instead of copying old one
			if (row < m_tileAmount.y - 1)
				m_tiles[column + row * m_tileAmount.x] = oldTiles[column + row * m_tileAmount.x - skippedTiles];
			else
			{
				m_tiles[column + row * m_tileAmount.x] = addedTile;
				skippedTiles++;
			}
		}
	}

	sectionTiles(tileSize);
	reAssign(tileSize);
}

bool TileMap::contains(sf::Vector2f &point)
{
	return m_mapRect.contains(point);
}

bool TileMap::contains(sf::Vector2i &point)
{
	return m_mapRect.contains(static_cast<sf::Vector2f>(point));
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = m_tileSet;

	// draw the vertex array
	target.draw(m_vertices, states);
}

sf::Vector2u& TileMap::getSize()
{
	return m_size;
}

sf::Vector2u& TileMap::getTileAmount()
{
	return m_tileAmount;
}
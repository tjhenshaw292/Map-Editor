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
			quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
			quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
			quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
			quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
		}

}

void TileMap::sectionTiles(sf::Vector2u &tileSize)
{
	m_mapRect.top = this->getPosition().y;
	m_mapRect.left = this->getPosition().x;
	m_mapRect.width = m_size.x;
	m_mapRect.height = m_size.y;
	for (unsigned int i = 0; i < m_tileAmount.x; ++i)
		for (unsigned int j = 0; j < m_tileAmount.y; ++j)
		{
			m_tiles[i + j * m_tileAmount.x].tileRect.top = j * tileSize.y;
			m_tiles[i + j * m_tileAmount.x].tileRect.left = i * tileSize.x + m_mapRect.left;
			m_tiles[i + j * m_tileAmount.x].tileRect.width = tileSize.x;
			m_tiles[i + j * m_tileAmount.x].tileRect.height = tileSize.y;
		}
}

int TileMap::getTileNumber(sf::Vector2i &position)
{
	for (auto &element : m_tiles)
	{
		if (element.tileRect.contains(static_cast<sf::Vector2f>(position)))
			return element.tileNumber;
	}
}

int TileMap::getTileIndex(sf::Vector2i &position)
{
	for (int i{ 0 }; i < m_tiles.size(); i++)
	{
		if (m_tiles[i].tileRect.contains(static_cast<sf::Vector2f>(position)))
			return i;
	}
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
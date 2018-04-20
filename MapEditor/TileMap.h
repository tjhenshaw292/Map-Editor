#pragma once

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
	enum TileProperty
	{
		NOTHING, BLOCKED, ACTION, WARP, SPECIAL, DOOR, JUMP_DOWN, JUMP_LEFT, JUMP_RIGHT
	};

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::VertexArray m_vertices;
	sf::Texture *m_tileSet;
	sf::Vector2u m_tileAmount;
	sf::Vector2u m_size;
	sf::FloatRect m_mapRect;
	struct Tile
	{
		int tileNumber;
		TileProperty tileProperty;
		sf::FloatRect tileRect;
	};

public:
	void create(sf::Texture *tileSet, sf::Vector2u &size, sf::Vector2u &tileSize);
	void reAssign(sf::Vector2u &tileSize);
	void sectionTiles(sf::Vector2u &tileSize);
	bool contains(sf::Vector2f &point);
	bool contains(sf::Vector2i &point);
	int getTileNumber(sf::Vector2f &position);
	int getTileIndex(sf::Vector2f &position);
	sf::Vector2u& getSize();
	sf::Vector2u& getTileAmount();
	std::vector<Tile> m_tiles;
};
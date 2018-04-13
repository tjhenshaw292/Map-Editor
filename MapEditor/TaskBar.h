#pragma once

class ToggleButton : public sf::Drawable, public sf::Transformable
{
	sf::FloatRect m_clickRect;
	sf::RectangleShape m_buttonShape;
	sf::Text m_buttonLetter;
	bool m_turnedOn;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	static sf::Color s_onColor;
	static sf::Color s_offColor;
public:
	ToggleButton(char buttonLetter, sf::Font &font);
	static sf::Vector2f s_buttonSize;
	bool buttonIsClicked(sf::Vector2f clickPosition);
	void toggle();
	bool isOn();
};

class TileDisplay : public sf::Drawable, public sf::Transformable
{
	sf::VertexArray m_vertices;
	sf::Texture *m_tileSet;
	sf::Vector2u &m_tileSize;
	sf::RectangleShape m_border;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	static float s_outlineThickness;
public:
	TileDisplay(sf::Texture *tileMap, sf::Vector2u &tileSize);
	static sf::Vector2f s_displaySize;
	void setTile(int tileNumber);
};

class TaskWindow
{
	sf::RenderWindow m_window;
	static sf::Color s_backGround;
	TileDisplay m_currentTileDisplay;
public:
	TaskWindow(sf::Vector2u size, sf::Texture *tileSet, sf::Vector2u &tileSize, sf::Font &font);
	ToggleButton m_BGToggle;
	ToggleButton m_FGToggle;
	ToggleButton m_maskToggle;
	ToggleButton m_lineToggle;
	ToggleButton m_properties;
	void display();
	void handleClick(sf::Vector2i clickPoint);
	void setCurrentTile(int tileNumber);
};
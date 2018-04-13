#include "stdafx.h"
#include "TaskBar.h"

class MapMaker;

ToggleButton::ToggleButton(char buttonLetter, sf::Font &font) :
	m_clickRect{ sf::Vector2f(0, 0), s_buttonSize },
	m_buttonShape{ s_buttonSize },
	m_buttonLetter{ buttonLetter, font, static_cast<unsigned int>(s_buttonSize.y) },
	m_turnedOn{ true }
{
	//Button Rect
	m_buttonShape.setFillColor(s_onColor);
	m_buttonShape.setOutlineColor(sf::Color::Black);
	m_buttonShape.setOutlineThickness(s_buttonSize.y / 20);

	//Button Letter
	m_buttonLetter.setFillColor(sf::Color::Black);
}

void ToggleButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// draw the vertex array
	target.draw(m_buttonShape, states);
	target.draw(m_buttonLetter, states);
}

bool ToggleButton::buttonIsClicked(sf::Vector2f clickPosition)
{
	if (m_clickRect.contains(clickPosition))
	{
		toggle();
		return true;
	}
	else
		return false;
}

void ToggleButton::toggle()
{
	m_turnedOn = m_turnedOn ? false : true;
	if (m_turnedOn)
		m_buttonShape.setFillColor(s_onColor);
	else
		m_buttonShape.setFillColor(s_offColor);
}

bool ToggleButton::isOn()
{
	return m_turnedOn;
}

TileDisplay::TileDisplay(sf::Texture *tileMap, sf::Vector2u &tileSize) :
	m_tileSet{ tileMap },
	m_tileSize{ tileSize },
	m_vertices{sf::Quads, 4}
{
	m_border.setSize(sf::Vector2f(200, 200));
	m_border.setFillColor(sf::Color::Transparent);
	m_border.setOutlineThickness(-s_outlineThickness);
	m_border.setOutlineColor(sf::Color::Red);
}

void TileDisplay::setTile(int tileNumber)
{
	// find its position in the tileset texture
	int tu = tileNumber % (m_tileSet->getSize().x / m_tileSize.x);
	int tv = tileNumber / (m_tileSet->getSize().x / m_tileSize.x);

	// define its 4 corners
	m_vertices[0].position = sf::Vector2f(s_outlineThickness, s_outlineThickness);
	m_vertices[1].position = sf::Vector2f(s_displaySize.x - s_outlineThickness, s_outlineThickness);
	m_vertices[2].position = sf::Vector2f(s_displaySize.x - s_outlineThickness, s_displaySize.y - s_outlineThickness);
	m_vertices[3].position = sf::Vector2f(s_outlineThickness, s_displaySize.y - s_outlineThickness);

	// define its 4 texture coordinates
	m_vertices[0].texCoords = sf::Vector2f(tu * static_cast<float>(m_tileSize.x), tv * static_cast<float>(m_tileSize.y));
	m_vertices[1].texCoords = sf::Vector2f((tu + 1) * static_cast<float>(m_tileSize.x), tv * static_cast<float>(m_tileSize.y));
	m_vertices[2].texCoords = sf::Vector2f((tu + 1) * static_cast<float>(m_tileSize.x), (tv + 1) * static_cast<float>(m_tileSize.y));
	m_vertices[3].texCoords = sf::Vector2f(tu * static_cast<float>(m_tileSize.x), (tv + 1) * static_cast<float>(m_tileSize.y));

}

void TileDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = m_tileSet;

	// draw the vertex array
	target.draw(m_border, states);
	target.draw(m_vertices, states);
}

TaskWindow::TaskWindow(sf::Vector2u size, sf::Texture *tileSet, sf::Vector2u &tileSize, sf::Font &font) :
	m_currentTileDisplay{ tileSet, tileSize },
	m_BGToggle{ 'B', font },
	m_FGToggle{ 'F', font },
	m_maskToggle{ 'M', font },
	m_lineToggle{ 'L', font },
	m_properties{ 'P', font }
{
	m_window.create(sf::VideoMode(size.x, size.y), "TaskWindow");
	m_currentTileDisplay.setPosition(0, 0);

	//Math
	sf::Vector2f allotedButtonSpace{ static_cast<float>(size.x) / 5, static_cast<float>(size.y) - TileDisplay::s_displaySize.y };
	sf::Vector2f buttonSize{ allotedButtonSpace.x * (5.0f / 6.0f), allotedButtonSpace.y * (5.0f / 6.0f) };
	sf::Vector2f offset{ (allotedButtonSpace.x - buttonSize.x) / 2, (allotedButtonSpace.y - buttonSize.y) / 2 };
	sf::Vector2f buttonScale{ buttonSize.x / ToggleButton::s_buttonSize.x, buttonSize.y / ToggleButton::s_buttonSize.y };

	std::vector<ToggleButton*> buttons{ &m_BGToggle, &m_FGToggle, &m_maskToggle, &m_lineToggle, &m_properties };

	for (int i{ 0 }; i < 5; i++)
	{
		buttons[i]->setScale(buttonScale);
		buttons[i]->setPosition(i * allotedButtonSpace.x + offset.x, TileDisplay::s_displaySize.y + offset.y);
	}
}

void TaskWindow::display()
{
	m_window.clear(s_backGround);
	m_window.draw(m_BGToggle);
	m_window.draw(m_FGToggle);
	m_window.draw(m_maskToggle);
	m_window.draw(m_lineToggle);
	m_window.draw(m_properties);
	m_window.draw(m_currentTileDisplay);
	m_window.display();
}

void TaskWindow::setCurrentTile(int tileNumber)
{
	m_currentTileDisplay.setTile(tileNumber);
}

sf::Color ToggleButton::s_onColor{ 116, 116, 116 };
sf::Color ToggleButton::s_offColor{ 202, 202, 202 };
sf::Vector2f ToggleButton::s_buttonSize{ 100, 100 };
sf::Vector2f TileDisplay::s_displaySize{ 200, 200 };
float TileDisplay::s_outlineThickness{ 5.0f };
sf::Color TaskWindow::s_backGround{ sf::Color::White };

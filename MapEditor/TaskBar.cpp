#include "stdafx.h"
#include "TaskBar.h"

TaskBar::TaskBar(unsigned int width)
{
	if (!m_font.loadFromFile("Carlito-Regular.ttf"))
		throw;
	m_backGround.setSize(sf::Vector2f(width, 50));
	m_backGround.setFillColor(sf::Color(195, 195, 195));
	m_lines.outline.setSize(sf::Vector2f(120, 35));
	m_lines.outline.setFillColor(sf::Color(195, 195, 195));
	m_lines.outline.setPosition(sf::Vector2f(100, 7.5f));
	m_lines.outline.setOutlineColor(sf::Color::Black);
	m_lines.outline.setOutlineThickness(1.7f);
	m_lines.text.setFont(m_font);
	m_lines.text.setCharacterSize(20);
	m_lines.text.setString("Lines");
	m_lines.text.setPosition(sf::Vector2f(130, 10));
	m_lines.turnedOn = true;
}

void TaskBar::toggle(TaskBar::Button &button)
{
	if (button.turnedOn)
	{
		button.outline.setFillColor(sf::Color(127, 127, 127));
		button.turnedOn = false;
	}
	else
	{
		button.outline.setFillColor(sf::Color(195, 195, 195));
		button.turnedOn = true;
	}
}

void TaskBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();
	
	// draw the vertex array
	target.draw(m_backGround, states);
	target.draw(m_lines.outline, states);
	target.draw(m_lines.text, states);
}



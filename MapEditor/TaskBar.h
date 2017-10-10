#pragma once

class TaskBar : public sf::Drawable, public sf::Transformable
{
	sf::RectangleShape m_backGround;
	struct Button
	{
		sf::RectangleShape outline;
		sf::Text text;
		bool turnedOn;
	};
	Button m_foreground;
	Button m_background;
	Button m_save;
	sf::Font m_font;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	TaskBar(unsigned int width);
	void toggle(Button &button);
	Button m_lines;
};
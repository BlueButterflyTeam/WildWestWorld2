#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <SFML\Graphics\Text.hpp>

#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RenderWindow.hpp>

class Button
{
private:
	sf::Text label;

public:
	Button(sf::Font& font, std::string label="Button", float x=0, float y=0)
	{
		this->label.setString(label);
		this->label.setFont(font);
		this->setPosition(sf::Vector2f(x, y));
	}

	~Button() {}

	void setMessage(std::string msg) { this->label.setString(msg); }
	void setFont(sf::Font& font) { this->label.setFont(font); }
	void setTextColor(sf::Color c) { this->label.setFillColor(c); }
	void setTextSize(unsigned int size) { this->label.setCharacterSize(size); }
	sf::Text getMessage() { return this->label; }
	void setPosition(float x, float y) { this->setPosition(sf::Vector2f(x, y)); }

	void setTextOption(sf::Font& font, sf::Color c, unsigned int size)
	{
		this->setFont(font);
		this->setTextColor(c);
		this->setTextSize(size);
	}

	void setPosition(sf::Vector2f pos)
	{
		this->label.setPosition(pos.x, pos.y - this->label.getCharacterSize() - 5);
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(this->label);
	}

	bool clicked(float x, float y)
	{
		return label.getGlobalBounds().contains(sf::Vector2f(x, y));
	}

	virtual void onClick()
	{

	}
};

#endif
#ifndef LOCATIONS_H
#define LOCATIONS_H

#include <SFML/Graphics/Text.hpp>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RenderWindow.hpp>

enum location_type
{
	shack,
	goldmine,
	bank,
	saloon
};

class Location;

static std::map <int, Location*> worldMap;
//static Location* worldMap[4];

class Location
{
private :
	sf::Sprite sprite;
	sf::Text name;

public:
	Location(sf::Texture& texture, sf::Font& font, std::string locationName)
	{
		this->name.setFont(font);
		this->name.setCharacterSize(20);
		this->name.setFillColor(sf::Color::Black);
		this->name.setString(locationName);

		this->sprite.setTexture(texture);
	}

	void scale(sf::Vector2f scale) { this->sprite.scale(scale); }
	void setSpriteColor(sf::Color color) { this->sprite.setColor(color); }
	sf::Vector2f getPosition() { return this->sprite.getPosition(); }
	void setPosition(float x, float y) { this->setPosition(sf::Vector2f(x, y)); }

	void setPosition(sf::Vector2f pos)
	{
		this->sprite.setPosition(pos);
		this->name.setPosition(pos.x, pos.y - this->name.getCharacterSize() - 5);
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(this->sprite);
		window.draw(this->name);
	}
};

//uncomment to send output to 'output.txt'
//#define TEXTOUTPUT




#endif
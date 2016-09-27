#ifndef ENTITY_H
#define ENTITY_H
//------------------------------------------------------------------------
//
//  Name:   BaseGameEntity.h
//
//  Desc:   Base class for a game object
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <string>
#include <SFML/Graphics/Text.hpp>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RenderWindow.hpp>

#include "Telegram.h"


class BaseGameEntity
{

private:

	//every entity must have a unique identifying number
	int          m_ID;

	//this is the next valid ID. Each time a BaseGameEntity is instantiated
	//this value is updated
	static int  m_iNextValidID;

	sf::Text message;
	sf::Sprite sprite;

	//this must be called within the constructor to make sure the ID is set
	//correctly. It verifies that the value passed to the method is greater
	//or equal to the next valid ID, before setting the ID and incrementing
	//the next valid ID
	void SetID(int val);

public:

	BaseGameEntity(int id, sf::Texture& texture, sf::Font& font, sf::Color c = sf::Color::Black, unsigned int size = 15)
	{
		SetID(id);

		setTextOption(font, c, size);
		this->message.setString("Hello world");

		this->sprite.setTexture(texture);
	}

	virtual ~BaseGameEntity() {}

	//all entities must implement an update function
	virtual void  Update() = 0;

	//all entities can communicate using messages. They are sent
	//using the MessageDispatcher singleton class
	virtual bool  HandleMessage(const Telegram& msg) = 0;

	int           ID()const { return m_ID; }

	void setMessage(std::string msg);
	void setFont(sf::Font& font) { this->message.setFont(font); }
	void setTextColor(sf::Color c) { this->message.setFillColor(c); }
	void setTextSize(unsigned int size) { this->message.setCharacterSize(size); }
	void scale(sf::Vector2f scale) { this->sprite.scale(scale); }
	void setSpriteColor(sf::Color color) { this->sprite.setColor(color); }
	sf::Text getMessage() { return this->message; }
	sf::Sprite getSprite() { return this->sprite; }
	void setPosition(float x, float y) { this->setPosition(sf::Vector2f(x, y)); }

	void setTextOption(sf::Font& font, sf::Color c, unsigned int size)
	{
		this->setFont(font);
		this->setTextColor(c);
		this->setTextSize(size);
	}

	void setPosition(sf::Vector2f pos)
	{
		this->sprite.setPosition(pos);
		this->message.setPosition(pos.x, pos.y + this->sprite.getGlobalBounds().height + 5);
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(this->sprite);
		window.draw(this->message);
	}
};



#endif



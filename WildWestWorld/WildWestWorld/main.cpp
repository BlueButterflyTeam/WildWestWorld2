#include <fstream>
#include <thread>
#include <chrono>
#include <time.h>
#include <map>

#include "Locations.h"
#include "Miner.h"
#include "DrunkMiner.h"
#include "MinersWife.h"
#include "ConsoleUtils.h"
#include "EntityNames.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "Button.h"
#include "StateButton.h"

#include <SFML\Graphics.hpp>

#define NB_NPC 3

std::ofstream os;

bool stopThread = true;

enum textures
{
	t_bob,
	t_elsa,
	t_mine,
	t_bank,
	t_house,
	t_saloon
};

void loop(BaseGameEntity* entity)
{
	while(!stopThread)
	{
		entity->Update();

		Dispatch->DispatchDelayedMessages();

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}


int main()
{
	//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
	os.open("output.txt");
#endif

	sf::Font font;
	if (!font.loadFromFile("../fonts/Roboto-Condensed.ttf"))
		return -1;


	sf::Texture textures[6];
	if (!textures[t_bob].loadFromFile("../sprites/bob.png"))
		return -1;
	if (!textures[t_elsa].loadFromFile("../sprites/elsa.png"))
		return -1;
	if (!textures[t_mine].loadFromFile("../sprites/mine.jpg"))
		return -1;
	if (!textures[t_bank].loadFromFile("../sprites/bank.png"))
		return -1;
	if (!textures[t_house].loadFromFile("../sprites/house.png"))
		return -1;
	if (!textures[t_saloon].loadFromFile("../sprites/saloon.png"))
		return -1;

	Location m_mine(textures[t_mine], font, "Mine");
	m_mine.setPosition(200, 100);
	m_mine.scale(sf::Vector2f(0.5f, 0.5f));

	Location m_bank(textures[t_bank], font, "Bank");
	m_bank.setPosition(550, 100);
	m_bank.scale(sf::Vector2f(0.25f, 0.25f));

	Location m_house(textures[t_house], font, "House");
	m_house.setPosition(900, 100);
	m_house.scale(sf::Vector2f(0.4f, 0.4f));

	Location m_saloon(textures[t_saloon], font, "Saloon");
	m_saloon.setPosition(1200, 100);
	m_saloon.scale(sf::Vector2f(0.17f, 0.17f));

	worldMap[goldmine] = &m_mine;
	worldMap[bank] = &m_bank;
	worldMap[shack] = &m_house;
	worldMap[saloon] = &m_saloon;

	//create a miner
	Miner* Bob = new Miner(ent_Miner_Bob, textures[t_bob], font);
	Bob->scale(sf::Vector2f(0.2f, 0.2f));

	//create his wife
	MinersWife* Elsa = new MinersWife(ent_Elsa, textures[t_elsa], font);
	Elsa->scale(sf::Vector2f(0.1f, 0.1f));

	//create a drunk miner
	DrunkMiner* Marley = new DrunkMiner(ent_DrunkMiner_Marley, textures[t_bob], font);
	Marley->setSpriteColor(sf::Color(156, 39, 176)); 
	Marley->scale(sf::Vector2f(0.2f, 0.2f));

	//register them with the entity manager
	EntityMgr->RegisterEntity(Bob);
	EntityMgr->RegisterEntity(Elsa);
	EntityMgr->RegisterEntity(Marley);

	Button* buttons[14] = {
		new Button(font, "Start", 700, 875),
		new Button(font, "Pause", 900, 875),
		new Button(font, "Bob", 10, 400),
		new Button(font, "Marley", 1450, 400),
		new StateButton(Bob, GoHomeAndSleepTilRested::Instance(), font, "Home", 10, 500),
		new StateButton(Bob, EnterMineAndDigForNugget::Instance(), font, "Mine", 10, 550),
		new StateButton(Bob, QuenchThirst::Instance(), font, "Saloon", 10, 600),
		new StateButton(Bob, VisitBankAndDepositGold::Instance(), font, "Bank", 10, 650),
		new StateButton(Bob, Fight::Instance(), font, "Fighting", 10, 700),
		new StateButton(Marley, HomeSweetHome::Instance(),font, "Home", 1450, 500),
		new StateButton(Marley, DigForNugget::Instance(), font, "Mine", 1450, 550),
		new StateButton(Marley, Drink::Instance(), font, "Saloon", 1450, 600),
		new StateButton(Marley, GoToBankToSaveGold::Instance(), font, "Bank", 1450, 650),
		new StateButton(Marley, Fighting::Instance(), font, "Fighting", 1450, 700),
	};

	for (int i = 3; i < 14; i++)
	{
		buttons[i]->setTextColor(sf::Color::Black);
	}
	for (int i = 0; i < 4; i++)
	{
		buttons[i]->setTextColor(sf::Color::Blue);
		buttons[i]->setTextSize(40);
	}

	std::thread threads[NB_NPC];
	
	sf::RenderWindow window(sf::VideoMode(1600, 900), "Wild West World");

	Bob->setPosition(150, window.getSize().y / 2);
	Elsa->setPosition(600, window.getSize().y / 2);
	Marley->setPosition(1000, window.getSize().y / 2);

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				if (!stopThread)
				{
					stopThread = true;
					for (int i = 0; i < NB_NPC; i++)
					{
						threads[i].join();
					}
				}
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					for (int i = 3; i < 14; i++)
					{
						if (buttons[i]->clicked(event.mouseButton.x, event.mouseButton.y))
						{
							buttons[i]->onClick();
						}
					}
					if (buttons[0]->clicked(event.mouseButton.x, event.mouseButton.y))
					{
						if (stopThread)
						{
							stopThread = false;
							threads[0] = std::thread(loop, Bob);
							threads[1] = std::thread(loop, Elsa);
							threads[2] = std::thread(loop, Marley);
						}
					}
					if (buttons[1]->clicked(event.mouseButton.x, event.mouseButton.y))
					{
						if (!stopThread)
						{
							stopThread = true;
							for (int i = 0; i < NB_NPC; i++)
							{
								threads[i].join();
							}
						}
					}
				}
			}
		}

		window.clear(sf::Color::White);

		for (auto it = worldMap.begin(); it != worldMap.end(); it++)
		{
			it->second->draw(window);
		}

		Bob->draw(window);
		Marley->draw(window);
		Elsa->draw(window);

		for (int i = 0; i < 14; i++)
		{
			buttons[i]->draw(window);
		}
		
		window.display();
	}

	return 0;
}
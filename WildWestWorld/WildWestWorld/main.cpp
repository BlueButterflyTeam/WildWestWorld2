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

#include <SFML\Graphics.hpp>

#define NB_NPC 3

std::ofstream os;

bool stopThread = false;
std::map <int, Location*> worldMap;

enum textures
{
	bob,
	elsa,
	mine
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


	sf::Texture textures[3];
	if (!textures[bob].loadFromFile("../sprites/Bob.png"))
		return -1;
	if (!textures[elsa].loadFromFile("../sprites/Elsa.jpg"))
		return -1;
	if (!textures[mine].loadFromFile("../sprites/mine.jpg"))
		return -1;

	Location mine(textures[mine], font, "Mine");
	mine.setPosition(200, 200);
	mine.scale(sf::Vector2f(0.5f, 0.5f));

	worldMap[goldmine] = &mine;

	//create a miner
	Miner* Bob = new Miner(ent_Miner_Bob, textures[0], font);
	Bob->scale(sf::Vector2f(0.2f, 0.2f));

	//create his wife
	MinersWife* Elsa = new MinersWife(ent_Elsa, textures[1], font);
	Elsa->scale(sf::Vector2f(0.5f, 0.5f));

	//create a drunk miner
	DrunkMiner* Marley = new DrunkMiner(ent_DrunkMiner_Marley, textures[0], font);
	Marley->setSpriteColor(sf::Color(156, 39, 176)); 
	Marley->scale(sf::Vector2f(0.2f, 0.2f));


	//register them with the entity manager
	EntityMgr->RegisterEntity(Bob);
	EntityMgr->RegisterEntity(Elsa);
	EntityMgr->RegisterEntity(Marley);

	std::thread threads[NB_NPC];

	threads[0] = std::thread(loop, Bob);
	threads[1] = std::thread(loop, Elsa);
	threads[2] = std::thread(loop, Marley);


	//sf::RenderWindow window(sf::VideoMode(1600, 900), "Wild West World");

	sf::Vector2f position(window.getSize().x/2, window.getSize().y/2);

	Bob->setPosition(position);
	Bob->setMessage("Test");

	Elsa->setPosition(sf::Vector2f(100, 0));

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				stopThread = true;
				window.close();
			}
		}

	//	window.clear(sf::Color::White);

		mine.draw(window);
		Bob->draw(window);
		Marley->draw(window);
		Elsa->draw(window);

	//	window.display();
	//}

	for (int i = 0; i < NB_NPC; i++)
	{
		threads[i].join();
	}

	return 0;
}
#include <fstream>
#include <thread>
#include <chrono>
#include <time.h>

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

void loop(BaseGameEntity* entity)
{
	for (int i = 0; i < 20; i++)
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

	sf::Texture texture;
	if (!texture.loadFromFile("../sprites/mine.jpg"))
		return -1;

	sf::Font font;
	if (!font.loadFromFile("../fonts/Roboto-Condensed.ttf"))
		return -1;


	//create a miner
	Miner* Bob = new Miner(ent_Miner_Bob, texture, font);

	//create his wife
	MinersWife* Elsa = new MinersWife(ent_Elsa, texture, font);

	////create a drunk miner
	DrunkMiner* Marley = new DrunkMiner(ent_DrunkMiner_Marley, texture, font);

	//register them with the entity manager
	EntityMgr->RegisterEntity(Bob);
	EntityMgr->RegisterEntity(Elsa);
	EntityMgr->RegisterEntity(Marley);

	std::thread threads[NB_NPC];


	if (true)
	{
		threads[0] = std::thread(loop, Bob);
		threads[1] = std::thread(loop, Elsa);
		threads[2] = std::thread(loop, Marley);


		for (int i = 0; i < NB_NPC; i++)
		{
			threads[i].join();
		}
	}
//	else
//	{
//		//run Bob and Elsa through a few Update calls
//		for (int i = 0; i<2; ++i)
//		{
//			Bob->Update();
//			Elsa->Update();
//			Marley->Update();
//
//			//dispatch any delayed messages
//			Dispatch->DispatchDelayedMessages();
//
//			//Sleep(800);
//		}
//	}
//
	//wait for a keypress before exiting
	PressAnyKeyToContinue();

	//sf::RenderWindow window(sf::VideoMode(1600, 900), "Wild West World");


	//sf::Text text, t2 = Bob->getMessage();

	//sf::Vector2f position(window.getSize().x/2, window.getSize().y/2);

	//Bob->setPosition(position);
	//Bob->scale(sf::Vector2f(0.5f, 0.5f));

	//Bob->setMessage("Test");

	//while (window.isOpen())
	//{
	//	sf::Event event;

	//	while (window.pollEvent(event))
	//	{
	//		if (event.type == sf::Event::Closed)
	//			window.close();
	//	}

	//	window.clear(sf::Color::White);


	//	//window.draw(Bob->getSprite());
	//	//window.draw(Bob->getMessage());
	//	Bob->draw(window);

	//	window.display();
	//}

	return 0;
}
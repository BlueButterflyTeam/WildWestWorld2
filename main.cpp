#include <fstream>
#include <thread>
#include <chrono>

#include "Locations.h"
#include "Miner.h"
#include "DrunkMiner.h"
#include "MinersWife.h"
#include "ConsoleUtils.h"
#include "EntityNames.h"

#define NB_NPC 2

std::ofstream os;

void loop(BaseGameEntity* entity)
{
	for (int i = 0; i < 1; i++)
	{
		entity->Update();

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}


int main()
{
//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
  os.open("output.txt");
#endif

  //create a miner
  Miner Bob(ent_Miner_Bob);

  //create a drunk miner
  DrunkMiner Marley(ent_DrunkMiner_Marley);

  //create his wife
  MinersWife Elsa(ent_Elsa);

  std::thread threads[NB_NPC];

  
  if (!true)
  {
	  threads[0] = std::thread(loop, &Bob);
	  threads[1] = std::thread(loop, &Elsa);
	  //threads[2] = std::thread(loop, &Marley);
	  

	  for (int i = 0; i < NB_NPC; i++)
	  {
		  threads[i].join();
	  }
  }
  else
  {
	  //run Bob and Elsa through a few Update calls
	  for (int i=0; i<2; ++i)
	  {
		  Bob.Update();
		  //Marley.Update();
		  Elsa.Update();

		  //Sleep(800);
	  }
  }

  //wait for a keypress before exiting
  PressAnyKeyToContinue();

  return 0;
}







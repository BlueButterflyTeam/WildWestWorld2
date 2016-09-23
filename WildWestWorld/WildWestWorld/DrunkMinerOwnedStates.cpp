#include "DrunkMinerOwnedStates.h"
#include "State.h"
#include "DrunkMiner.h"
#include "Locations.h"
#include "EntityNames.h"
#include "ConsoleUtils.h"
#include "Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "CrudeTimer.h"

#include <iostream>

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

//--------------------------------------methods for DigForNugget

DigForNugget* DigForNugget::Instance()
{
	static DigForNugget instance;

	return &instance;
}

void DigForNugget::Enter(DrunkMiner* pMiner)
{
	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine
	if (pMiner->Location() != goldmine)
	{
		if (pMiner->IsDrunk())
		{
			writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Walkin' to the goldmine", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);

			Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
				pMiner->ID(),        //ID of sender
				ent_Miner_Bob,            //ID of recipient
				Msg_AtTheMine,   //the message
				NO_ADDITIONAL_INFO);
		}
		else
		{
			writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Walkin' to the goldmine", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		}

		pMiner->ChangeLocation(goldmine);
	}
	//if drunk & another miner is there, pick a fight
	//if drunk and alone, taking someone else's pickaxe
	if (pMiner->IsDrunk())
	{
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Oh, great, forgot mah pickaxe! Well, just gonna take that one!", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	}
}

void DigForNugget::Execute(DrunkMiner* pMiner)
{
	//if the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	pMiner->AddToGoldCarried(1);

	pMiner->IncreaseFatigue();

	if (pMiner->IsDrunk())
	{
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Pickin' up a nugget", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	}
	else
	{
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Pickin' up a nugget", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}

	//if no more pickaxe but someone else's coming start fighting
	/*if ()
	{
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "No, that's my axe!";
		pMiner->GetFSM()->ChangeState(Fight::Instance());
	}*/

	//if enough gold mined, go and put it in the bank
	if (pMiner->PocketsFull())
	{
		pMiner->GetDrunkFSM()->ChangeState(GoToBankToSaveGold::Instance());
	}

	if (pMiner->Thirsty())
	{
		pMiner->GetDrunkFSM()->ChangeState(Drink::Instance());
	}
}

void DigForNugget::Exit(DrunkMiner* pMiner)
{
	if (pMiner->IsDrunk())
	{
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Let's go an' settle this somewhere else!", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	}
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Ah'm leavin' the goldmine with mah pockets full o' sweet gold", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

bool DigForNugget::OnMessage(DrunkMiner* pMiner, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case Msg_AtTheMine:
		writeOnConsole("Message handled by " + GetNameOfEntity(pMiner->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Test!", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		return true;
	}//end switch
	return false; //send message to global message handler
}


//----------------------------------------methods for GoToBankToSaveGold

GoToBankToSaveGold* GoToBankToSaveGold::Instance()
{
	static GoToBankToSaveGold instance;

	return &instance;
}

void GoToBankToSaveGold::Enter(DrunkMiner* pMiner)
{
	//on entry the miner makes sure he is located at the bank
	if (pMiner->Location() != bank)
	{
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Goin' to the bank. Yes siree", FOREGROUND_BLUE | FOREGROUND_INTENSITY);

		pMiner->ChangeLocation(bank);
	}
}

void GoToBankToSaveGold::Execute(DrunkMiner* pMiner)
{
	//deposit the gold
	pMiner->AddToWealth(pMiner->GoldCarried());

	pMiner->SetGoldCarried(0);
	
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Depositing gold. Total savings now: " + std::to_string(pMiner->Wealth()), FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	//wealthy enough to have a well earned rest?
	if (pMiner->Wealth() >= ComfortLevel)
	{
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": WooHoo! Rich enough for now. Back home to mah li'lle lady", FOREGROUND_BLUE | FOREGROUND_INTENSITY);

		pMiner->GetDrunkFSM()->ChangeState(HomeSweetHome::Instance());
	}

	//otherwise get more gold
	else
	{
		pMiner->GetDrunkFSM()->ChangeState(DigForNugget::Instance());
	}

}

void GoToBankToSaveGold::Exit(DrunkMiner* pMiner)
{
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Leavin' the bank", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

bool GoToBankToSaveGold::OnMessage(DrunkMiner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}


//----------------------------------------methods for HomeSweetHome

HomeSweetHome* HomeSweetHome::Instance()
{
	static HomeSweetHome instance;

	return &instance;
}

void HomeSweetHome::Enter(DrunkMiner* pMiner)
{
	if (pMiner->Location() != shack)
	{
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Walkin' home", FOREGROUND_BLUE | FOREGROUND_INTENSITY);

		pMiner->ChangeLocation(shack);
	}
}

void HomeSweetHome::Execute(DrunkMiner* pMiner)
{
	//if miner is not fatigued start to dig for nuggets again.
	if (!pMiner->Fatigued())
	{
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": What a God darn fantastic nap! Time to find more gold", FOREGROUND_BLUE | FOREGROUND_INTENSITY);

		pMiner->GetDrunkFSM()->ChangeState(DigForNugget::Instance());
	}

	else
	{
		//sleep
		pMiner->DecreaseFatigue();

		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": ZZZZ... ", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
}

void HomeSweetHome::Exit(DrunkMiner* pMiner)
{
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Leaving the house", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

bool HomeSweetHome::OnMessage(DrunkMiner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}

//------------------------------------------------methods for Drink

Drink* Drink::Instance()
{
	static Drink instance;

	return &instance;
}

void Drink::Enter(DrunkMiner* pMiner)
{
	if (pMiner->Location() != saloon)
	{
		pMiner->ChangeLocation(saloon);
		if (pMiner->IsDrunk())
		{
			writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Boy, ah sure is thusty! Walking to the saloon", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		}
		else
		{
			writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Boy, ah sure is thusty! Walking to the saloon", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		}

		if (pMiner->IsDrunk())
		{
			Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
				pMiner->ID(),        //ID of sender
				ent_Miner_Bob,          //ID of recipient
				Msg_AtTheSaloon,   //the message
				NO_ADDITIONAL_INFO);
		}
	}
}

void Drink::Execute(DrunkMiner* pMiner)
{
	if (pMiner->Thirsty())
	{
		pMiner->BuyAndDrinkAWhiskey();

		//if drank too much, miner gets drunk
		pMiner->GetDrunk();

		if (pMiner->IsDrunk())
		{
			writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Ouh! Head spinnin'!", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		}
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": That's mighty fine sippin' liquer", FOREGROUND_BLUE | FOREGROUND_INTENSITY);

		pMiner->GetDrunkFSM()->ChangeState(DigForNugget::Instance());
	}

	else
	{
		writeOnConsole("\nERROR!\nERROR!\nERROR!", FOREGROUND_RED | FOREGROUND_INTENSITY);
	}
}

void Drink::Exit(DrunkMiner* pMiner)
{
	if (pMiner->IsDrunk())
	{
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Let's go an' settle this somewhere else!", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	}
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Leaving the saloon, feelin' good", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

bool Drink::OnMessage(DrunkMiner* pMiner, const Telegram& msg)
{
		switch (msg.Msg)
		{
		case Msg_AtTheSaloon:
			if (pMiner->IsDrunk())
			{
				writeOnConsole("Message handled by " + GetNameOfEntity(pMiner->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": I challenge you to a duel!", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				pMiner->GetDrunkFSM()->ChangeState(Fighting::Instance());
				return true;
			}

		}//end switch
		return false; //send message to global message handler	
}

//------------------------------------------------methods for Fight
Fighting* Fighting::Instance()
{
	static Fighting instance;

	return &instance;
}

void Fighting::Enter(DrunkMiner* pMiner)
{
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
		pMiner->ID(),        //ID of sender
		ent_Miner_Bob,            //ID of recipient
		Msg_LetsFight,   //the message
		NO_ADDITIONAL_INFO);
}

void Fighting::Execute(DrunkMiner* pMiner)
{
	pMiner->IncreaseFatigue();

	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Am not gonna loose this fight!", FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	if (pMiner->Thirsty())
	{
		pMiner->GetDrunkFSM()->ChangeState(Drink::Instance());
	}
	if (pMiner->Fatigued())
	{
		pMiner->GetDrunkFSM()->ChangeState(HomeSweetHome::Instance());
	}
}

void Fighting::Exit(DrunkMiner* pMiner)
{
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
		pMiner->ID(),        //ID of sender
		ent_Miner_Bob,            //ID of recipient
		Msg_LeavingFight,   //the message
		NO_ADDITIONAL_INFO);

}

bool Fighting::OnMessage(DrunkMiner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}
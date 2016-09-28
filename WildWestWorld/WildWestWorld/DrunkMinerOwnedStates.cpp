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

void DigForNugget::Enter(BaseGameEntity* pMiner)
{
	DrunkMiner* miner = (DrunkMiner*)pMiner;

	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine
	if (miner->getLocation() != goldmine)
	{
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Walkin' to the goldmine", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->setMessage("Walkin' to the goldmine");

		if (miner->IsDrunk())
		{
			Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
				miner->ID(),        //ID of sender
				ent_Miner_Bob,            //ID of recipient
				Msg_AtTheMine,   //the message
				NO_ADDITIONAL_INFO);
		}

		miner->ChangeLocation(goldmine);
	}

	//if drunk & another miner is there, pick a fight
	//if drunk and alone, taking someone else's pickaxe
	if (miner->IsDrunk())
	{
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Oh, great, forgot mah pickaxe! Well, just gonna take that one!", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->setMessage("Oh, great, forgot mah pickaxe! Well, just gonna take that one!");
	}
}

void DigForNugget::Execute(BaseGameEntity* pMiner)
{
	DrunkMiner* miner = (DrunkMiner*)pMiner;

	//if the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	miner->AddToGoldCarried(1);

	miner->IncreaseFatigue();

		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Pickin' up a nugget", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->setMessage("Pickin' up a nugget");
	
	//if enough gold mined, go and put it in the bank
	if (miner->PocketsFull())
	{
		miner->GetFSM()->ChangeState(GoToBankToSaveGold::Instance());
	}

	if (miner->Thirsty())
	{
		miner->GetFSM()->ChangeState(Drink::Instance());
	}
}

void DigForNugget::Exit(BaseGameEntity* pMiner)
{
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Ah'm leavin' the goldmine", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	pMiner->setMessage("Ah'm leavin' the goldmine");
}

bool DigForNugget::OnMessage(BaseGameEntity* pMiner, const Telegram& msg)
{
	DrunkMiner* miner = (DrunkMiner*)pMiner;

	if (miner->IsDrunk())
	{
		switch (msg.Msg)
		{
		case Msg_AtTheMine:
			writeOnConsole("Message handled by " + GetNameOfEntity(miner->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			writeOnConsole(GetNameOfEntity(miner->ID()) + ": Hey! That's mah pickaxe!!", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
			miner->GetFSM()->ChangeState(Fighting::Instance());
			return true;

		case Msg_LetsFight:
			writeOnConsole("Message handled by " + GetNameOfEntity(miner->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			writeOnConsole(GetNameOfEntity(miner->ID()) + ": Oh boy!", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
			miner->GetFSM()->ChangeState(Fighting::Instance());
			return true;
		}//end switch
	}
	return false; //send message to global message handler
}


//----------------------------------------methods for GoToBankToSaveGold

GoToBankToSaveGold* GoToBankToSaveGold::Instance()
{
	static GoToBankToSaveGold instance;

	return &instance;
}

void GoToBankToSaveGold::Enter(BaseGameEntity* pMiner)
{
	DrunkMiner* miner = (DrunkMiner*)pMiner;

	//on entry the miner makes sure he is located at the bank
	if (miner->getLocation() != bank)
	{
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Goin' to the bank. Yes siree", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->setMessage("Goin' to the bank. Yes siree");

		miner->ChangeLocation(bank);
	}
}

void GoToBankToSaveGold::Execute(BaseGameEntity* pMiner)
{
	DrunkMiner* miner = (DrunkMiner*)pMiner;

	//deposit the gold
	miner->AddToWealth(miner->GoldCarried());

	miner->SetGoldCarried(0);
	
	writeOnConsole(GetNameOfEntity(miner->ID()) + ": Depositing gold. Total savings now: " + std::to_string(miner->Wealth()), FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	miner->setMessage("Depositing gold. Total savings now: " + std::to_string(miner->Wealth()));

	//wealthy enough to have a well earned rest?
	if (miner->Wealth() >= ComfortLevel)
	{
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": WooHoo! Rich enough for now. Back home to ... nobody ...", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->setMessage("WooHoo! Rich enough for now. Back home to ... nobody ...");

		miner->GetFSM()->ChangeState(HomeSweetHome::Instance());
	}

	//otherwise get more gold
	else
	{
		miner->GetFSM()->ChangeState(DigForNugget::Instance());
	}

}

void GoToBankToSaveGold::Exit(BaseGameEntity* pMiner)
{
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Leavin' the bank", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	pMiner->setMessage("Leavin' the bank");
}

bool GoToBankToSaveGold::OnMessage(BaseGameEntity* pMiner, const Telegram& msg)
{
	return false; //send message to global message handler
}


//----------------------------------------methods for HomeSweetHome

HomeSweetHome* HomeSweetHome::Instance()
{
	static HomeSweetHome instance;

	return &instance;
}

void HomeSweetHome::Enter(BaseGameEntity* pMiner)
{
	DrunkMiner* miner = (DrunkMiner*)pMiner;

	miner->GetSober();
	if (miner->getLocation() != shack)
	{
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Walkin' home", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->setMessage("Walkin' home");

		miner->ChangeLocation(shack);
	}
}

void HomeSweetHome::Execute(BaseGameEntity* pMiner)
{
	DrunkMiner* miner = (DrunkMiner*)pMiner;

	//if miner is not fatigued start to dig for nuggets again.
	if (!miner->Fatigued())
	{
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": What a God darn fantastic nap! Time to find more gold", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->setMessage("What a God darn fantastic nap! Time to find more gold");

		miner->GetFSM()->ChangeState(DigForNugget::Instance());
	}

	else
	{
		//sleep
		miner->DecreaseFatigue();

		writeOnConsole(GetNameOfEntity(miner->ID()) + ": ZZZZ... ", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->setMessage("ZZZZ... ");
	}
}

void HomeSweetHome::Exit(BaseGameEntity* pMiner)
{
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Leaving the house", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	pMiner->setMessage("Leaving the house");
}

bool HomeSweetHome::OnMessage(BaseGameEntity* pMiner, const Telegram& msg)
{
	return false; //send message to global message handler
}


//------------------------------------------------methods for Drink

Drink* Drink::Instance()
{
	static Drink instance;

	return &instance;
}

void Drink::Enter(BaseGameEntity* pMiner)
{
	DrunkMiner* miner = (DrunkMiner*)pMiner;

	if (miner->getLocation() != saloon)
	{
		miner->ChangeLocation(saloon);
			writeOnConsole(GetNameOfEntity(miner->ID()) + ": Boy, ah sure is thusty! Walking to the saloon", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
			miner->setMessage("Boy, ah sure is thusty! Walking to the saloon");

			if (miner->IsDrunk())
			{
				Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
					miner->ID(),        //ID of sender
					ent_Miner_Bob,            //ID of recipient
					Msg_AtTheSaloon,   //the message
					NO_ADDITIONAL_INFO);
			}
	}
}

void Drink::Execute(BaseGameEntity* pMiner)
{
	DrunkMiner* miner = (DrunkMiner*)pMiner;

	if (miner->Thirsty())
	{
		miner->BuyAndDrinkAWhiskey();

		//if drank too much, miner gets drunk
		miner->GetDrunk();

		if (miner->IsDrunk())
		{
			writeOnConsole(GetNameOfEntity(miner->ID()) + ": Ouh! Head spinnin'!", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
			miner->setMessage("Ouh! Head spinnin'!");
		}
		else
		{
			writeOnConsole(GetNameOfEntity(miner->ID()) + ": That's mighty fine sippin' liquer", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
			miner->setMessage("That's mighty fine sippin' liquer");
		}

		miner->GetFSM()->ChangeState(DigForNugget::Instance());
	}

	else
	{
		miner->GetFSM()->ChangeState(DigForNugget::Instance());
	}
}

void Drink::Exit(BaseGameEntity* pMiner)
{
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Leaving the saloon, feelin' good", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	pMiner->setMessage("Leaving the saloon, feelin' good");
}

bool Drink::OnMessage(BaseGameEntity* pMiner, const Telegram& msg)
{
	DrunkMiner* miner = (DrunkMiner*)pMiner;

	if (miner->IsDrunk())
	{
		switch (msg.Msg)
		{
		case Msg_AtTheSaloon:
			writeOnConsole(GetNameOfEntity(miner->ID()) + ": Just finishing mah drink!", FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			miner->BuyAndDrinkAWhiskey();
			writeOnConsole("Message handled by " + GetNameOfEntity(miner->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			writeOnConsole(GetNameOfEntity(miner->ID()) + ": Oh, hey Bob!", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
			miner->GetFSM()->ChangeState(Fighting::Instance());
			return true;

		case Msg_LetsFight:
			writeOnConsole("Message handled by " + GetNameOfEntity(miner->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			writeOnConsole(GetNameOfEntity(miner->ID()) + ": Oh, boy!", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
			miner->GetFSM()->ChangeState(Fighting::Instance());
			return true;
		}//end switch
	}
	return false; //send message to global message handler
}


//------------------------------------------------methods for Fighting
Fighting* Fighting::Instance()
{
	static Fighting instance;

	return &instance;
}

void Fighting::Enter(BaseGameEntity* pMiner)
{
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Let's go an' settle this somewhere else!", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	pMiner->setMessage("Let's go an' settle this somewhere else!");

	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
		pMiner->ID(),        //ID of sender
		ent_Miner_Bob,            //ID of recipient
		Msg_LetsFight,   //the message
		NO_ADDITIONAL_INFO);
}

void Fighting::Execute(BaseGameEntity* pMiner)
{
	DrunkMiner* miner = (DrunkMiner*)pMiner;

	miner->IncreaseFatigue();

	writeOnConsole(GetNameOfEntity(miner->ID()) + ": Am not gonna loose this fight!", FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	miner->setMessage("Am not gonna loose this fight!");

	if (miner->Thirsty())
	{
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Too thirsty!", FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		miner->setMessage("Too thirsty!");

		miner->GetFSM()->ChangeState(Drink::Instance());
	}
	if (miner->Fatigued())
	{
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Too tired!", FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		miner->setMessage("Too tired!");

		miner->GetFSM()->ChangeState(HomeSweetHome::Instance());
	}
}

void Fighting::Exit(BaseGameEntity* pMiner)
{
	DrunkMiner* miner = (DrunkMiner*)pMiner;

	miner->GetSober();
	Dispatch->DispatchMessage(0.1, //time delay
		miner->ID(),        //ID of sender
		ent_Miner_Bob,            //ID of recipient
		Msg_LeavingFight,   //the message
		NO_ADDITIONAL_INFO);

}

bool Fighting::OnMessage(BaseGameEntity* pMiner, const Telegram& msg)
{
	DrunkMiner* miner = (DrunkMiner*)pMiner;

	switch (msg.Msg)
	{
	case Msg_LeavingFight:
		miner->GetFSM()->ChangeState(DigForNugget::Instance());
		writeOnConsole("Message handled by " + GetNameOfEntity(miner->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Sorry for that, dude!", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		return true;
	}//end switch
	return false;
}

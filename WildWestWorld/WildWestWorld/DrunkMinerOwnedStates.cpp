#include "DrunkMinerOwnedStates.h"
#include "State.h"
#include "DrunkMiner.h"
#include "Locations.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;


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
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Walkin' to the goldmine";

		pMiner->ChangeLocation(goldmine);
	}
	//if drunk & another miner is there, pick a fight
	//if drunk and alone, taking someone else's pickaxe
	if ()
	{
		if ()
		{
			cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Uh oh, not HIM!";
			pMiner->GetFSM()->ChangeState(Fight::Instance());
		}
		else
		{
			cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Oh, great, forgot mah pickaxe! Well, just gonna that one!";
		}
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

	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Pickin' up a nugget";

	//if no more pickaxe but someone else's coming start fighting
	if ()
	{
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "No, that's my axe!";
		pMiner->GetFSM()->ChangeState(Fight::Instance());
	}

	//if enough gold mined, go and put it in the bank
	if (pMiner->PocketsFull())
	{
		pMiner->GetFSM()->ChangeState(GoToBankToSaveGold::Instance());
	}

	if (pMiner->Thirsty())
	{
		pMiner->GetFSM()->ChangeState(Drink::Instance());
	}
}


void DigForNugget::Exit(DrunkMiner* pMiner)
{
	if ()
	{
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ":" << "Let's go an' settle this somewhere else!";
	}
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Ah'm leavin' the goldmine with mah pockets full o' sweet gold";
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
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Goin' to the bank. Yes siree";

		pMiner->ChangeLocation(bank);
	}
}


void GoToBankToSaveGold::Execute(DrunkMiner* pMiner)
{
	//deposit the gold
	pMiner->AddToWealth(pMiner->GoldCarried());

	pMiner->SetGoldCarried(0);

	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
		<< "Depositing gold. Total savings now: " << pMiner->Wealth();

	//wealthy enough to have a well earned rest?
	if (pMiner->Wealth() >= ComfortLevel)
	{
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
			<< "WooHoo! Rich enough for now. Back home to mah li'lle lady";

		pMiner->GetFSM()->ChangeState(HomeSweetHome::Instance());
	}

	//otherwise get more gold
	else
	{
		pMiner->GetFSM()->ChangeState(DigForNugget::Instance());
	}

}


void GoToBankToSaveGold::Exit(DrunkMiner* pMiner)
{
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Leavin' the bank";
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
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Walkin' home";

		pMiner->ChangeLocation(shack);
	}
}

void HomeSweetHome::Execute(DrunkMiner* pMiner)
{
	//if miner is not fatigued start to dig for nuggets again.
	if (!pMiner->Fatigued())
	{
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
			<< "What a God darn fantastic nap! Time to find more gold";

		pMiner->GetFSM()->ChangeState(DigForNugget::Instance());
	}

	else
	{
		//sleep
		pMiner->DecreaseFatigue();

		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "ZZZZ... ";
	}
}

void HomeSweetHome::Exit(DrunkMiner* pMiner)
{
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Leaving the house";
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

		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Boy, ah sure is thusty! Walking to the saloon";
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
			cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Ouh! Head spinnin'!";
			//if another miner is there, pick a fight
			if ()
			{
				cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "I challenge you to a duel!";
				pMiner->GetFSM()->ChangeState(Fight::Instance());
			}
		}
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "That's mighty fine sippin' liquer";

		pMiner->GetFSM()->ChangeState(DigForNugget::Instance());
	}

	else
	{
		cout << "\nERROR!\nERROR!\nERROR!";
	}
}

void Drink::Exit(DrunkMiner* pMiner)
{
	if (pMiner->IsDrunk())
	{
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ":" << "Let's go an' settle this somewhere else!";
	}
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Leaving the saloon, feelin' good";
}


//------------------------------------------------methods for Fight
Fight* Fight::Instance()
{
	static Fight instance;

	return &instance;
}

void Fight::Enter(DrunkMiner* pMiner)
{

}

void Fight::Execute(DrunkMiner* pMiner)
{

}

void Fight::Exit(DrunkMiner* pMiner)
{

}
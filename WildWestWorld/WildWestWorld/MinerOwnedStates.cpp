#include "MinerOwnedStates.h"
#include "State.h"
#include "Miner.h"
#include "Locations.h"
#include "EntityNames.h"
#include "ConsoleUtils.h"
#include "Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "CrudeTimer.h"

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

//--------------------------------------methods for EnterMineAndDigForNugget
EnterMineAndDigForNugget* EnterMineAndDigForNugget::Instance()
{
	static EnterMineAndDigForNugget instance;

	return &instance;
}

void EnterMineAndDigForNugget::Enter(BaseGameEntity* pMiner)
{
	Miner* miner = (Miner*)pMiner;

	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine
	if (miner->getLocation() != goldmine)
	{
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Walkin' to the goldmine", FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->setMessage("Walking to the goldmine");

		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			miner->ID(),        //ID of sender
			ent_DrunkMiner_Marley,            //ID of recipient
			Msg_AtTheMine,   //the message
			NO_ADDITIONAL_INFO);

		miner->ChangeLocation(goldmine);
	}
}

void EnterMineAndDigForNugget::Execute(BaseGameEntity* pMiner)
{
	Miner* miner = (Miner*)pMiner;

	//if the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	miner->AddToGoldCarried(1);

	miner->IncreaseFatigue();

	writeOnConsole(GetNameOfEntity(miner->ID()) + ": Pickin' up a nugget", FOREGROUND_RED | FOREGROUND_INTENSITY);
	miner->setMessage("Pickin' a nugget");

	//if enough gold mined, go and put it in the bank
	if (miner->PocketsFull())
	{
		miner->GetFSM()->ChangeState(VisitBankAndDepositGold::Instance());
	}

	if (miner->Thirsty())
	{
		miner->GetFSM()->ChangeState(QuenchThirst::Instance());
	}
}

void EnterMineAndDigForNugget::Exit(BaseGameEntity* pMiner)
{
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Ah'm leavin' the goldmine with mah pockets full o' sweet gold", FOREGROUND_RED | FOREGROUND_INTENSITY);
	pMiner->setMessage("Ah'm leavin' the goldmine with mah pockets full o' sweet gold");
}

bool EnterMineAndDigForNugget::OnMessage(BaseGameEntity* pMiner, const Telegram& msg)
{
	Miner* miner = (Miner*)pMiner;

	switch (msg.Msg)
	{
	case Msg_AtTheMine:
		writeOnConsole("Message handled by " + GetNameOfEntity(miner->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Marley, that's my axe!", FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->GetFSM()->ChangeState(Fight::Instance());
		return true;

	case Msg_LetsFight:
		writeOnConsole("Message handled by " + GetNameOfEntity(miner->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Oh boy!", FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->GetFSM()->ChangeState(Fight::Instance());
		return true;
	}//end switch
	return false; //send message to global message handler
}

//----------------------------------------methods for VisitBankAndDepositGold
VisitBankAndDepositGold* VisitBankAndDepositGold::Instance()
{
	static VisitBankAndDepositGold instance;

	return &instance;
}

void VisitBankAndDepositGold::Enter(BaseGameEntity* pMiner)
{
	Miner* miner = (Miner*)pMiner;

	//on entry the miner makes sure he is located at the bank
	if (miner->getLocation() != bank)
	{
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Goin' to the bank. Yes siree", FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->setMessage("Goin' to the bank. Yes siree");

		miner->ChangeLocation(bank);
	}
}

void VisitBankAndDepositGold::Execute(BaseGameEntity* pMiner)
{
	Miner* miner = (Miner*)pMiner;

	//deposit the gold
	miner->AddToWealth(miner->GoldCarried());

	miner->SetGoldCarried(0);

	writeOnConsole(GetNameOfEntity(miner->ID()) + ": Depositing gold. Total savings now: " + std::to_string(miner->Wealth()), FOREGROUND_RED | FOREGROUND_INTENSITY);
	miner->setMessage("Depositing gold. Total savings now: " + std::to_string(miner->Wealth()));

	//wealthy enough to have a well earned rest?
	if (miner->Wealth() >= ComfortLevel)
	{
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": WooHoo! Rich enough for now. Back home to mah li'lle lady", FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->setMessage("WooHoo! Rich enough for now. Back home to mah li'lle lady");

		miner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
	}

	//otherwise get more gold
	else
	{
		miner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	}

}

void VisitBankAndDepositGold::Exit(BaseGameEntity* pMiner)
{
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Leavin' the bank", FOREGROUND_RED | FOREGROUND_INTENSITY);
	pMiner->setMessage("Leavin' the bank");
}

bool VisitBankAndDepositGold::OnMessage(BaseGameEntity* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}

//----------------------------------------methods for GoHomeAndSleepTilRested
GoHomeAndSleepTilRested* GoHomeAndSleepTilRested::Instance()
{
	static GoHomeAndSleepTilRested instance;

	return &instance;
}

void GoHomeAndSleepTilRested::Enter(BaseGameEntity* pMiner)
{
	Miner* miner = (Miner*)pMiner;

	if (miner->getLocation() != shack)
	{
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Walkin' home", FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->setMessage("Walkin' home");

		miner->ChangeLocation(shack);

		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			miner->ID(),        //ID of sender
			ent_Elsa,            //ID of recipient
			Msg_HiHoneyImHome,   //the message
			NO_ADDITIONAL_INFO);
	}
}

void GoHomeAndSleepTilRested::Execute(BaseGameEntity* pMiner)
{
	Miner* miner = (Miner*)pMiner;

	//if miner is not fatigued start to dig for nuggets again.
	if (!miner->Fatigued())
	{
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": What a God darn fantastic nap! Time to find more gold", FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->setMessage("What a God darn fantastic nap! Time to find more gold");

		miner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	}

	else
	{
		//sleep
		miner->DecreaseFatigue();

		writeOnConsole(GetNameOfEntity(miner->ID()) + ": ZZZZ... ", FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->setMessage("ZZZZ...");
	}
}

void GoHomeAndSleepTilRested::Exit(BaseGameEntity* pMiner)
{
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Leaving the house", FOREGROUND_RED | FOREGROUND_INTENSITY);
	pMiner->setMessage("Leaving the house");
}

bool GoHomeAndSleepTilRested::OnMessage(BaseGameEntity* pMiner, const Telegram& msg)
{
	Miner* miner = (Miner*)pMiner;

	switch (msg.Msg)
	{
	case Msg_StewReady:

		writeOnConsole("Message handled by " + GetNameOfEntity(miner->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Okay Hun, ahm a comin'!", FOREGROUND_RED | FOREGROUND_INTENSITY);

		miner->GetFSM()->ChangeState(EatStew::Instance());

		return true;

	}//end switch

	return false; //send message to global message handler
}

//------------------------------------------------methods for QuenchThirst
QuenchThirst* QuenchThirst::Instance()
{
	static QuenchThirst instance;

	return &instance;
}

void QuenchThirst::Enter(BaseGameEntity* pMiner)
{
	Miner* miner = (Miner*)pMiner;

	if (miner->getLocation() != saloon)
	{
		miner->ChangeLocation(saloon);

		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Boy, ah sure is thusty! Walking to the saloon", FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->setMessage("Boy, ah sure is thusty! Walking to the saloon");

		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			miner->ID(),        //ID of sender
			ent_DrunkMiner_Marley,            //ID of recipient
			Msg_AtTheSaloon,   //the message
			NO_ADDITIONAL_INFO);
	}
}

void QuenchThirst::Execute(BaseGameEntity* pMiner)
{
	Miner* miner = (Miner*)pMiner;

	if (miner->Thirsty())
	{
		miner->BuyAndDrinkAWhiskey();

		writeOnConsole(GetNameOfEntity(miner->ID()) + ": That's mighty fine sippin' liquer", FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->setMessage("That's mighty fine sippin' liquer");

		miner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	}

	else
	{
		writeOnConsole("\nERROR!\nERROR!\nERROR!", FOREGROUND_RED | FOREGROUND_INTENSITY);
	}
}

void QuenchThirst::Exit(BaseGameEntity* pMiner)
{
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Leaving the saloon, feelin' good", FOREGROUND_RED | FOREGROUND_INTENSITY);
	pMiner->setMessage("Leaving the saloon, feelin' good");
}

bool QuenchThirst::OnMessage(BaseGameEntity* pMiner, const Telegram& msg)
{
	Miner* miner = (Miner*)pMiner;

	switch (msg.Msg)
	{
	case Msg_AtTheSaloon:
		writeOnConsole("Message handled by " + GetNameOfEntity(miner->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Uh oh, it's Marley, seems drunk, again!", FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->GetFSM()->ChangeState(Fight::Instance());
		return true;

	case Msg_LetsFight:
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Just finishing mah drink!", FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->BuyAndDrinkAWhiskey();
		writeOnConsole("Message handled by " + GetNameOfEntity(miner->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Oh boy!", FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->GetFSM()->ChangeState(Fight::Instance());
		return true;
	}//end switch
	return false; //send message to global message handler
}

//------------------------------------------------------------------------EatStew
EatStew* EatStew::Instance()
{
	static EatStew instance;

	return &instance;
}

void EatStew::Enter(BaseGameEntity* pMiner)
{
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Smells Reaaal goood Elsa!", FOREGROUND_RED | FOREGROUND_INTENSITY);
	pMiner->setMessage("Smells Reaaal goood Elsa!");
}

void EatStew::Execute(BaseGameEntity* pMiner)
{
	Miner* miner = (Miner*)pMiner;

	writeOnConsole(GetNameOfEntity(miner->ID()) + ": Tastes real good too!", FOREGROUND_RED | FOREGROUND_INTENSITY);
	miner->setMessage("Tastes real good too!");

	miner->GetFSM()->RevertToPreviousState();
}

void EatStew::Exit(BaseGameEntity* pMiner)
{
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Thankya li'lle lady. Ah better get back to whatever ah wuz doin'", FOREGROUND_RED | FOREGROUND_INTENSITY);
	pMiner->setMessage("Thankya li'lle lady. Ah better get back to whatever ah wuz doin'");
}

bool EatStew::OnMessage(BaseGameEntity* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}

//------------------------------------------------methods for Fight
Fight* Fight::Instance()
{
	static Fight instance;

	return &instance;
}

void Fight::Enter(BaseGameEntity* pMiner)
{
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
		pMiner->ID(),        //ID of sender
		ent_DrunkMiner_Marley,            //ID of recipient
		Msg_LetsFight,   //the message
		NO_ADDITIONAL_INFO);
}

void Fight::Execute(BaseGameEntity* pMiner)
{
	Miner* miner = (Miner*)pMiner;

	miner->IncreaseFatigue();

	writeOnConsole(GetNameOfEntity(miner->ID()) + ": Am not gonna loose this fight!", FOREGROUND_RED | FOREGROUND_INTENSITY);
	miner->setMessage("Am not gonna loose this fight!");

	if (miner->Thirsty())
	{
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Too thirsty!", FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->setMessage("Too thirsty!");
		miner->GetFSM()->ChangeState(QuenchThirst::Instance());
	}
	if (miner->Fatigued())
	{
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Too tired!", FOREGROUND_RED | FOREGROUND_INTENSITY);
		miner->setMessage("Too tired!");
		miner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
	}
}

void Fight::Exit(BaseGameEntity* pMiner)
{
	Dispatch->DispatchMessage(0.1, //time delay
		pMiner->ID(),        //ID of sender
		ent_DrunkMiner_Marley,            //ID of recipient
		Msg_LeavingFight,   //the message
		NO_ADDITIONAL_INFO);

}

bool Fight::OnMessage(BaseGameEntity* pMiner, const Telegram& msg)
{
	Miner* miner = (Miner*)pMiner;

	switch (msg.Msg)
	{
	case Msg_LeavingFight:
		miner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
		writeOnConsole("Message handled by " + GetNameOfEntity(miner->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		writeOnConsole(GetNameOfEntity(miner->ID()) + ": Sorry for that, dude!", FOREGROUND_RED | FOREGROUND_INTENSITY);
		return true;
	}//end switch
	return false;
}
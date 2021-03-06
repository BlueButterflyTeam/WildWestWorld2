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

void EnterMineAndDigForNugget::Enter(Miner* pMiner)
{
	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine
	if (pMiner->Location() != goldmine)
	{
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Walkin' to the goldmine", FOREGROUND_RED | FOREGROUND_INTENSITY);

		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pMiner->ID(),        //ID of sender
			ent_DrunkMiner_Marley,            //ID of recipient
			Msg_AtTheMine,   //the message
			NO_ADDITIONAL_INFO);

		pMiner->ChangeLocation(goldmine);
	}
}

void EnterMineAndDigForNugget::Execute(Miner* pMiner)
{
	//if the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	pMiner->AddToGoldCarried(1);

	pMiner->IncreaseFatigue();

	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Pickin' up a nugget", FOREGROUND_RED | FOREGROUND_INTENSITY);

	//if enough gold mined, go and put it in the bank
	if (pMiner->PocketsFull())
	{
		pMiner->GetFSM()->ChangeState(VisitBankAndDepositGold::Instance());
	}

	if (pMiner->Thirsty())
	{
		pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
	}
}

void EnterMineAndDigForNugget::Exit(Miner* pMiner)
{
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Ah'm leavin' the goldmine with mah pockets full o' sweet gold", FOREGROUND_RED | FOREGROUND_INTENSITY);
}

bool EnterMineAndDigForNugget::OnMessage(Miner* pMiner, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case Msg_AtTheMine:
		writeOnConsole("Message handled by " + GetNameOfEntity(pMiner->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Marley, that's my axe!", FOREGROUND_RED | FOREGROUND_INTENSITY);
		pMiner->GetFSM()->ChangeState(Fight::Instance());
		return true;

	case Msg_LetsFight:
		writeOnConsole("Message handled by " + GetNameOfEntity(pMiner->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Oh boy!", FOREGROUND_RED | FOREGROUND_INTENSITY);
		pMiner->GetFSM()->ChangeState(Fight::Instance());
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

void VisitBankAndDepositGold::Enter(Miner* pMiner)
{
	//on entry the miner makes sure he is located at the bank
	if (pMiner->Location() != bank)
	{
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Goin' to the bank. Yes siree", FOREGROUND_RED | FOREGROUND_INTENSITY);

		pMiner->ChangeLocation(bank);
	}
}

void VisitBankAndDepositGold::Execute(Miner* pMiner)
{
	//deposit the gold
	pMiner->AddToWealth(pMiner->GoldCarried());

	pMiner->SetGoldCarried(0);

	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Depositing gold. Total savings now: " + std::to_string(pMiner->Wealth()), FOREGROUND_RED | FOREGROUND_INTENSITY);

	//wealthy enough to have a well earned rest?
	if (pMiner->Wealth() >= ComfortLevel)
	{
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": WooHoo! Rich enough for now. Back home to mah li'lle lady", FOREGROUND_RED | FOREGROUND_INTENSITY);

		pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
	}

	//otherwise get more gold
	else
	{
		pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	}

}

void VisitBankAndDepositGold::Exit(Miner* pMiner)
{
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Leavin' the bank", FOREGROUND_RED | FOREGROUND_INTENSITY);
}

bool VisitBankAndDepositGold::OnMessage(Miner* pMiner, const Telegram& msg)
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

void GoHomeAndSleepTilRested::Enter(Miner* pMiner)
{
	if (pMiner->Location() != shack)
	{
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Walkin' home", FOREGROUND_RED | FOREGROUND_INTENSITY);

		pMiner->ChangeLocation(shack);

		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pMiner->ID(),        //ID of sender
			ent_Elsa,            //ID of recipient
			Msg_HiHoneyImHome,   //the message
			NO_ADDITIONAL_INFO);
	}
}

void GoHomeAndSleepTilRested::Execute(Miner* pMiner)
{
	//if miner is not fatigued start to dig for nuggets again.
	if (!pMiner->Fatigued())
	{
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": What a God darn fantastic nap! Time to find more gold", FOREGROUND_RED | FOREGROUND_INTENSITY);

		pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	}

	else
	{
		//sleep
		pMiner->DecreaseFatigue();

		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": ZZZZ... ", FOREGROUND_RED | FOREGROUND_INTENSITY);
	}
}

void GoHomeAndSleepTilRested::Exit(Miner* pMiner)
{
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Leaving the house", FOREGROUND_RED | FOREGROUND_INTENSITY);
}

bool GoHomeAndSleepTilRested::OnMessage(Miner* pMiner, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case Msg_StewReady:

		writeOnConsole("Message handled by " + GetNameOfEntity(pMiner->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Okay Hun, ahm a comin'!", FOREGROUND_RED | FOREGROUND_INTENSITY);

		pMiner->GetFSM()->ChangeState(EatStew::Instance());

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

void QuenchThirst::Enter(Miner* pMiner)
{
	if (pMiner->Location() != saloon)
	{
		pMiner->ChangeLocation(saloon);

		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Boy, ah sure is thusty! Walking to the saloon", FOREGROUND_RED | FOREGROUND_INTENSITY);

		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pMiner->ID(),        //ID of sender
			ent_DrunkMiner_Marley,            //ID of recipient
			Msg_AtTheSaloon,   //the message
			NO_ADDITIONAL_INFO);
	}
}

void QuenchThirst::Execute(Miner* pMiner)
{
	if (pMiner->Thirsty())
	{
		pMiner->BuyAndDrinkAWhiskey();

		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": That's mighty fine sippin' liquer", FOREGROUND_RED | FOREGROUND_INTENSITY);

		pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	}

	else
	{
		writeOnConsole("\nERROR!\nERROR!\nERROR!", FOREGROUND_RED | FOREGROUND_INTENSITY);
	}
}

void QuenchThirst::Exit(Miner* pMiner)
{
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Leaving the saloon, feelin' good", FOREGROUND_RED | FOREGROUND_INTENSITY);
}

bool QuenchThirst::OnMessage(Miner* pMiner, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case Msg_AtTheSaloon:
		writeOnConsole("Message handled by " + GetNameOfEntity(pMiner->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Uh oh, it's Marley, seems drunk, again!", FOREGROUND_RED | FOREGROUND_INTENSITY);
		pMiner->GetFSM()->ChangeState(Fight::Instance());
		return true;

	case Msg_LetsFight:
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Just finishing mah drink!", FOREGROUND_RED | FOREGROUND_INTENSITY);
		pMiner->BuyAndDrinkAWhiskey();
		writeOnConsole("Message handled by " + GetNameOfEntity(pMiner->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Oh boy!", FOREGROUND_RED | FOREGROUND_INTENSITY);
		pMiner->GetFSM()->ChangeState(Fight::Instance());
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

void EatStew::Enter(Miner* pMiner)
{
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Smells Reaaal goood Elsa!", FOREGROUND_RED | FOREGROUND_INTENSITY);
}

void EatStew::Execute(Miner* pMiner)
{
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Tastes real good too!", FOREGROUND_RED | FOREGROUND_INTENSITY);

	pMiner->GetFSM()->RevertToPreviousState();
}

void EatStew::Exit(Miner* pMiner)
{
	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Thankya li'lle lady. Ah better get back to whatever ah wuz doin'", FOREGROUND_RED | FOREGROUND_INTENSITY);
}

bool EatStew::OnMessage(Miner* pMiner, const Telegram& msg)
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

void Fight::Enter(Miner* pMiner)
{
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
		pMiner->ID(),        //ID of sender
		ent_DrunkMiner_Marley,            //ID of recipient
		Msg_LetsFight,   //the message
		NO_ADDITIONAL_INFO);
}

void Fight::Execute(Miner* pMiner)
{
	pMiner->IncreaseFatigue();

	writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Am not gonna loose this fight!", FOREGROUND_RED | FOREGROUND_INTENSITY);

	if (pMiner->Thirsty())
	{
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Too thirsty!", FOREGROUND_RED | FOREGROUND_INTENSITY);
		pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
	}
	if (pMiner->Fatigued())
	{
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Too tired!", FOREGROUND_RED | FOREGROUND_INTENSITY);
		pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
	}
}

void Fight::Exit(Miner* pMiner)
{
	Dispatch->DispatchMessage(0.1, //time delay
		pMiner->ID(),        //ID of sender
		ent_DrunkMiner_Marley,            //ID of recipient
		Msg_LeavingFight,   //the message
		NO_ADDITIONAL_INFO);

}

bool Fight::OnMessage(Miner* pMiner, const Telegram& msg)
{
	switch (msg.Msg)
	{
	case Msg_LeavingFight:
		pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
		writeOnConsole("Message handled by " + GetNameOfEntity(pMiner->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		writeOnConsole(GetNameOfEntity(pMiner->ID()) + ": Sorry for that, dude!", FOREGROUND_RED | FOREGROUND_INTENSITY);
		return true;
	}//end switch
	return false;
}
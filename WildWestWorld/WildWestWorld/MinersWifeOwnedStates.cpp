#include "MinersWifeOwnedStates.h"
#include "MinerOwnedStates.h"
#include "MinersWife.h"
#include "Locations.h"
#include "CrudeTimer.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "EntityNames.h"

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

//-----------------------------------------------------------------------Global state

WifesGlobalState* WifesGlobalState::Instance()
{
	static WifesGlobalState instance;

	return &instance;
}


void WifesGlobalState::Execute(BaseGameEntity* pWife)
{
	MinersWife* wife = (MinersWife*)pWife;

	//1 in 10 chance of needing the bathroom (provided she is not already
	//in the bathroom)
	if ((RandFloat() < 0.1) &&
		!wife->GetFSM()->isInState(*VisitBathroom::Instance()))
	{
		wife->GetFSM()->ChangeState(VisitBathroom::Instance());
	}
}

bool WifesGlobalState::OnMessage(BaseGameEntity* pWife, const Telegram& msg)
{
	MinersWife* wife = (MinersWife*)pWife;

	switch (msg.Msg)
	{
	case Msg_HiHoneyImHome:
	{
		writeOnConsole("Message handled by " + GetNameOfEntity(wife->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		
		writeOnConsole(GetNameOfEntity(wife->ID()) + ": Hi honey. Let me make you some of mah fine country stew", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		wife->setMessage("Hi honey. Let me make you some of mah fine country stew");

		wife->GetFSM()->ChangeState(CookStew::Instance());
	}

	return true;

	}//end switch

	return false;
}

//-------------------------------------------------------------------------DoHouseWork

DoHouseWork* DoHouseWork::Instance()
{
	static DoHouseWork instance;

	return &instance;
}


void DoHouseWork::Enter(BaseGameEntity* wife)
{
	writeOnConsole(GetNameOfEntity(wife->ID()) + ": Time to do some more housework!", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	wife->setMessage("Time to do some more housework!");
}


void DoHouseWork::Execute(BaseGameEntity* wife)
{
	switch (RandInt(0, 2))
	{
		case 0:
			writeOnConsole(GetNameOfEntity(wife->ID()) + ": Moppin' the floor", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			wife->setMessage("Moppin' the floor");

			break;
		case 1:
			writeOnConsole(GetNameOfEntity(wife->ID()) + ": Washin' the dishes", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			wife->setMessage("Washin' the dishes");

			break;
		case 2:
			writeOnConsole(GetNameOfEntity(wife->ID()) + ": Makin' the bed", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			wife->setMessage("Makin' the bed");

			break;
	}
}

void DoHouseWork::Exit(BaseGameEntity* wife)
{
}

bool DoHouseWork::OnMessage(BaseGameEntity* wife, const Telegram& msg)
{
	return false;
}

//------------------------------------------------------------------------VisitBathroom

VisitBathroom* VisitBathroom::Instance()
{
	static VisitBathroom instance;

	return &instance;
}


void VisitBathroom::Enter(BaseGameEntity* wife)
{
	writeOnConsole(GetNameOfEntity(wife->ID()) + ": Walkin' to the can. Need to powda mah pretty li'lle nose", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	wife->setMessage("Walkin' to the can. Need to powda mah pretty li'lle nose");
}


void VisitBathroom::Execute(BaseGameEntity* pWife)
{
	MinersWife* wife = (MinersWife*)pWife;

	writeOnConsole(GetNameOfEntity(wife->ID()) + ": Ahhhhhh! Sweet relief!", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	wife->setMessage("Ahhhhhh! Sweet relief!");

	wife->GetFSM()->RevertToPreviousState();
}

void VisitBathroom::Exit(BaseGameEntity* wife)
{
	writeOnConsole(GetNameOfEntity(wife->ID()) + ": Leavin' the Jon", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	wife->setMessage("Leavin' the Jon");
}


bool VisitBathroom::OnMessage(BaseGameEntity* wife, const Telegram& msg)
{
	return false;
}


//------------------------------------------------------------------------CookStew

CookStew* CookStew::Instance()
{
	static CookStew instance;

	return &instance;
}


void CookStew::Enter(BaseGameEntity* pWife)
{
	MinersWife* wife = (MinersWife*)pWife;

	//if not already cooking put the stew in the oven
	if (!wife->Cooking())
	{
		writeOnConsole(GetNameOfEntity(wife->ID()) + ": Putting the stew in the oven", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		wife->setMessage("Putting the stew in the oven");

		//send a delayed message myself so that I know when to take the stew
		//out of the oven
		Dispatch->DispatchMessage(1.5,                  //time delay
			wife->ID(),           //sender ID
			wife->ID(),           //receiver ID
			Msg_StewReady,        //msg
			NO_ADDITIONAL_INFO);

		wife->SetCooking(true);
	}
}


void CookStew::Execute(BaseGameEntity* wife)
{
	writeOnConsole(GetNameOfEntity(wife->ID()) + ": Fussin' over food", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	wife->setMessage("Fussin' over food");
}

void CookStew::Exit(BaseGameEntity* wife)
{
	writeOnConsole(GetNameOfEntity(wife->ID()) + ": Puttin' the stew on the table", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	wife->setMessage("Puttin' the stew on the table");
}


bool CookStew::OnMessage(BaseGameEntity* pWife, const Telegram& msg)
{
	MinersWife* wife = (MinersWife*)pWife;

	switch (msg.Msg)
	{
	case Msg_StewReady:
	{
		writeOnConsole("Message received by " + GetNameOfEntity(wife->ID()) + " at time: " + std::to_string(Clock->GetCurrentTime()), BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

		writeOnConsole(GetNameOfEntity(wife->ID()) + ": StewReady! Lets eat", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		wife->setMessage("StewReady! Lets eat");

		//let hubby know the stew is ready
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
			wife->ID(),
			ent_Miner_Bob,
			Msg_StewReady,
			NO_ADDITIONAL_INFO);

		wife->SetCooking(false);

		wife->GetFSM()->ChangeState(DoHouseWork::Instance());
	}

	return true;

	}//end switch

	return false;
}
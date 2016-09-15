#include "MinersWifeOwnedStates.h"
#include "MinersWife.h"
#include "Locations.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

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


void WifesGlobalState::Execute(MinersWife* wife)
{
	//1 in 10 chance of needing the bathroom
	if (RandFloat() < 0.1)
	{
		wife->GetFSM()->ChangeState(VisitBathroom::Instance());
	}
}

//---------------------------------------DoHouseWork

DoHouseWork* DoHouseWork::Instance()
{
	static DoHouseWork instance;

	return &instance;
}


void DoHouseWork::Enter(MinersWife* wife)
{
}


void DoHouseWork::Execute(MinersWife* wife)
{
	switch (RandInt(0, 2))
	{
	case 0:
		writeOnConsole(GetNameOfEntity(wife->ID()) + ": Moppin' the floor", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case 1:
		writeOnConsole(GetNameOfEntity(wife->ID()) + ": Washin' the dishes", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case 2:
		writeOnConsole(GetNameOfEntity(wife->ID()) + ": Makin' the bed", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	}
}

void DoHouseWork::Exit(MinersWife* wife)
{
}



//------------------------------------------------------------------------VisitBathroom
VisitBathroom* VisitBathroom::Instance()
{
	static VisitBathroom instance;

	return &instance;
}


void VisitBathroom::Enter(MinersWife* wife)
{
	writeOnConsole(GetNameOfEntity(wife->ID()) + ": Walkin' to the can. Need to powda mah pretty li'lle nose", FOREGROUND_GREEN | FOREGROUND_INTENSITY);	
}


void VisitBathroom::Execute(MinersWife* wife)
{
	writeOnConsole(GetNameOfEntity(wife->ID()) + ": Ahhhhhh! Sweet relief!", FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	wife->GetFSM()->RevertToPreviousState();
}

void VisitBathroom::Exit(MinersWife* wife)
{
	writeOnConsole(GetNameOfEntity(wife->ID()) + ": Leavin' the Jon", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	
}
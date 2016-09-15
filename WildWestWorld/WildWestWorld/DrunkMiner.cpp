#include "DrunkMiner.h"
#include "ConsoleUtils.h"

//-----------------------------------------------------------------------------
void DrunkMiner::Update()
{
	//SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	m_iThirst += 1;

	m_pStateMachine->Update();
}
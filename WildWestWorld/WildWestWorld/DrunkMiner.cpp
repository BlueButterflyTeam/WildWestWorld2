#include "DrunkMiner.h"
#include "ConsoleUtils.h"

bool DrunkMiner::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

//-----------------------------------------------------------------------------
void DrunkMiner::Update()
{
	//SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	m_iThirst += 1;

	m_pStateMachine->Update();
}
#ifndef DRUNK_MINER_H
#define DRUNK_MINER_H
//------------------------------------------------------------------------
//
//  Name:   Miner.h
//
//  Desc:   A class defining a goldminer.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <string>
#include <cassert>

#include "Miner.h"

class DrunkMiner : public Miner
{
private:
	bool m_iDrunkness;

public:
	DrunkMiner(int id) : Miner(id)
	{
		/*delete this->m_pStateMachine;

		this->m_pStateMachine = new StateMachine<DrunkMiner>(this);

		this->m_pStateMachine->SetCurrentState(GoHomeAndSleepTilRested::Instance());*/
	}

	~DrunkMiner()
	{
		//delete this->m_pStateMachine;
	}

	virtual void Update();

	virtual void BuyAndDrinkAWhiskey() { m_iThirst = 0; m_iMoneyInBank -= 2; }
};

#endif DRUNK_MINER_H
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
#include "DrunkMinerOwnedStates.h"

class DrunkMiner : public Miner
{
private:
	bool m_bIsDrunk = false;

	//StateMachine* m_pDrunkFSM;

public:
	DrunkMiner(int id, sf::Texture& texture, sf::Font& font, sf::Color c = sf::Color::Black, unsigned int size = 15) : Miner(id, texture, font, c, size)
	{
		//m_pDrunkFSM = new StateMachine(this);
		m_pStateMachine->SetCurrentState(DigForNugget::Instance());
	}

	~DrunkMiner()
	{
		//delete m_pDrunkFSM;
	}

	virtual void Update();

	virtual bool  HandleMessage(const Telegram& msg);

	virtual void BuyAndDrinkAWhiskey() { m_iThirst = 0; m_iMoneyInBank -= 2; }

	bool IsDrunk() { return m_bIsDrunk; }

	void GetDrunk() { m_bIsDrunk = true; }

	void GetSober() { m_bIsDrunk = false; }

	//StateMachine* GetDrunkFSM() const{ return m_pStateMachine; }
};

#endif DRUNK_MINER_H
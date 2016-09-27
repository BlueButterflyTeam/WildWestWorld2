#pragma once
#ifndef MINER_OWNED_STATES_H
#define MINER_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   MinerOwnedStates.h
//
//  Desc:   All the states that can be assigned to the Miner class
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include "State.h"


class Miner;
struct Telegram;


//------------------------------------------------------------------------
//
//  In this state the miner will walk to a goldmine and pick up a nugget
//  of gold. If the miner already has a nugget of gold he'll change state
//  to VisitBankAndDepositGold. If he gets thirsty he'll change state
//  to QuenchThirst
//------------------------------------------------------------------------
class EnterMineAndDigForNugget : public State
{
private:

	EnterMineAndDigForNugget() {}

	//copy ctor and assignment should be private
	EnterMineAndDigForNugget(const EnterMineAndDigForNugget&);
	EnterMineAndDigForNugget& operator=(const EnterMineAndDigForNugget&);

public:

	static EnterMineAndDigForNugget* Instance();

public:

	virtual void Enter(BaseGameEntity* miner);

	virtual void Execute(BaseGameEntity* miner);

	virtual void Exit(BaseGameEntity* miner);

	virtual bool OnMessage(BaseGameEntity* agent, const Telegram& msg);

};

//------------------------------------------------------------------------
//
//  Entity will go to a bank and deposit any nuggets he is carrying. If the 
//  miner is subsequently wealthy enough he'll walk home, otherwise he'll
//  keep going to get more gold
//------------------------------------------------------------------------
class VisitBankAndDepositGold : public State
{
private:

	VisitBankAndDepositGold() {}

	//copy ctor and assignment should be private
	VisitBankAndDepositGold(const VisitBankAndDepositGold&);
	VisitBankAndDepositGold& operator=(const VisitBankAndDepositGold&);

public:

	static VisitBankAndDepositGold* Instance();

	virtual void Enter(BaseGameEntity* miner);

	virtual void Execute(BaseGameEntity* miner);

	virtual void Exit(BaseGameEntity* miner);

	virtual bool OnMessage(BaseGameEntity* agent, const Telegram& msg);
};


//------------------------------------------------------------------------
//
//  miner will go home and sleep until his fatigue is decreased
//  sufficiently
//------------------------------------------------------------------------
class GoHomeAndSleepTilRested : public State
{
private:

	GoHomeAndSleepTilRested() {}

	//copy ctor and assignment should be private
	GoHomeAndSleepTilRested(const GoHomeAndSleepTilRested&);
	GoHomeAndSleepTilRested& operator=(const GoHomeAndSleepTilRested&);

public:

	static GoHomeAndSleepTilRested* Instance();

	virtual void Enter(BaseGameEntity* miner);

	virtual void Execute(BaseGameEntity* miner);

	virtual void Exit(BaseGameEntity* miner);

	virtual bool OnMessage(BaseGameEntity* agent, const Telegram& msg);
};


//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
class QuenchThirst : public State
{
private:

	QuenchThirst() {}

	//copy ctor and assignment should be private
	QuenchThirst(const QuenchThirst&);
	QuenchThirst& operator=(const QuenchThirst&);

public:

	static QuenchThirst* Instance();

	virtual void Enter(BaseGameEntity* miner);

	virtual void Execute(BaseGameEntity* miner);

	virtual void Exit(BaseGameEntity* miner);

	virtual bool OnMessage(BaseGameEntity* agent, const Telegram& msg);
};


//------------------------------------------------------------------------
//
//  this is implemented as a state blip. The miner eats the stew, gives
//  Elsa some compliments and then returns to his previous state
//------------------------------------------------------------------------
class EatStew : public State
{
private:

	EatStew() {}

	//copy ctor and assignment should be private
	EatStew(const EatStew&);
	EatStew& operator=(const EatStew&);

public:

	//this is a singleton
	static EatStew* Instance();

	virtual void Enter(BaseGameEntity* miner);

	virtual void Execute(BaseGameEntity* miner);

	virtual void Exit(BaseGameEntity* miner);

	virtual bool OnMessage(BaseGameEntity* agent, const Telegram& msg);
};

//------------------------------------------------------------------------
//
//	When drinking there's a probability our drunken miner will get drunk.
//	If another miner is here then, they'll start fighting, otherwise he'll
//	just go to the mine drunk.
//------------------------------------------------------------------------
class Fight : public State
{
private:

	Fight() {}

	//copy ctor and assignment should be private
	Fight(const Fight&);
	Fight& operator=(const Fight&);

public:

	static Fight* Instance();

	virtual void Enter(BaseGameEntity* miner);

	virtual void Execute(BaseGameEntity* miner);

	virtual void Exit(BaseGameEntity* miner);

	virtual bool OnMessage(BaseGameEntity* agent, const Telegram& msg);
};

#endif
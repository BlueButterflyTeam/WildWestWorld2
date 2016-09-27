#ifndef DRUNK_MINER_OWNED_STATES_H
#define DRUNK_MINER_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   DrunkMinerOwnedStates.h
//
//  Desc:   All the states that can be assigned to the Drunk Miner class
//
//  Author: Valryo 2016
//
//------------------------------------------------------------------------
#include "State.h"

class DrunkMiner;

//------------------------------------------------------------------------
//
//  In this state the drunken miner will walk to a goldmine and pick up a 
//  nugget of gold. If the miner already has a nugget of gold he'll change 
//  state to VisitBankAndDepositGold. If he gets thirsty he'll change 
//  state to QuenchThirst. If he is drunk and Bob is here, they'll pick
//	up a fight.
//------------------------------------------------------------------------
class DigForNugget : public State
{
private:

	DigForNugget() {}

	//copy ctor and assignment should be private
	DigForNugget(const DigForNugget&);
	DigForNugget& operator=(const DigForNugget&);

public:

	static DigForNugget* Instance();

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
class GoToBankToSaveGold : public State
{
private:

	GoToBankToSaveGold() {}

	//copy ctor and assignment should be private
	GoToBankToSaveGold(const GoToBankToSaveGold&);
	GoToBankToSaveGold& operator=(const GoToBankToSaveGold&);

public:

	static GoToBankToSaveGold* Instance();

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
class HomeSweetHome : public State
{
private:

	HomeSweetHome() {}

	//copy ctor and assignment should be private
	HomeSweetHome(const HomeSweetHome&);
	HomeSweetHome& operator=(const HomeSweetHome&);

public:

	static HomeSweetHome* Instance();

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
class Drink : public State
{
private:

	Drink() {}

	//copy ctor and assignment should be private
	Drink(const Drink&);
	Drink& operator=(const Drink&);

public:

	static Drink* Instance();

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
class Fighting : public State
{
private:

	Fighting() {}

	//copy ctor and assignment should be private
	Fighting(const Fighting&);
	Fighting& operator=(const Fighting&);

public:

	static Fighting* Instance();

	virtual void Enter(BaseGameEntity* miner);

	virtual void Execute(BaseGameEntity* miner);

	virtual void Exit(BaseGameEntity* miner);

	virtual bool OnMessage(BaseGameEntity* agent, const Telegram& msg);
};


#endif

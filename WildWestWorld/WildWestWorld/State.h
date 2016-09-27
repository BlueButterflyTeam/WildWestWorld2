#ifndef STATE_H
#define STATE_H
//------------------------------------------------------------------------
//
//  Name:   State.h
//
//  Desc:   abstract base class to define an interface for a state
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
struct Telegram;
class BaseGameEntity;

class State
{
public:

	virtual ~State() {}

	//this will execute when the state is entered
	virtual void Enter(BaseGameEntity*) = 0;

	//this is the states normal update function
	virtual void Execute(BaseGameEntity*) = 0;

	//this will execute when the state is exited. 
	virtual void Exit(BaseGameEntity*) = 0;

	//this executes if the agent receives a message from the 
	//message dispatcher
	virtual bool OnMessage(BaseGameEntity*, const Telegram&) = 0;
};

#endif
#ifndef STATEBUTTON_H
#define SATEBUTTON_H

#include "Button.h"
#include "State.h"

template <class entity_type>
class StateButton : public Button
{
private :
	State state;
	Miner* miner;

public :
	StateButton(Miner* miner, State state, sf::Font& font, std::string label = "Button", float x = 0, float y = 0) : Button (font, label, x, y)
	{
		this->state = state;
		this->miner = miner;
	}

	void onClick()
	{
		
	}
};

#endif
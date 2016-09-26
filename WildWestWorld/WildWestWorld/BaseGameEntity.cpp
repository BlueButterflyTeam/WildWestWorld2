#include "BaseGameEntity.h"
#include <cassert>
#include <sstream>
#include <vector>

int BaseGameEntity::m_iNextValidID = 0;

void split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

//----------------------------- SetID -----------------------------------------
//
//  this must be called within each constructor to make sure the ID is set
//  correctly. It verifies that the value passed to the method is greater
//  or equal to the next valid ID, before setting the ID and incrementing
//  the next valid ID
//-----------------------------------------------------------------------------
void BaseGameEntity::SetID(int val)
{
	//make sure the val is equal to or greater than the next available ID
	assert((val >= m_iNextValidID) && "<BaseGameEntity::SetID>: invalid ID");

	m_ID = val;

	m_iNextValidID = m_ID + 1;
}

void BaseGameEntity::setMessage(std::string msg)
{
	std::string tmp = this->message.getString(), last, middle;
	std::vector<std::string> str = split(tmp, '\n');

	tmp.clear();

	if (!str.empty())
	{
		auto it = str.end();
		
		it--;
		middle = *it;

		if(str.size() > 1)
		{
			it--;
			last = *it;
		}
	}
	
	tmp = last + "\n" + middle + "\n" + msg;

	this->message.setString(tmp);
}

void split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
}


std::vector<std::string> split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

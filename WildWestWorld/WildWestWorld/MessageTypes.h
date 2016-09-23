#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
	Msg_HiHoneyImHome,
	Msg_StewReady,
	Msg_AtTheMine,
	Msg_AtTheSaloon,
	Msg_LetsFight,
	Msg_LeavingFight
};


inline std::string MsgToStr(int msg)
{
	switch (msg)
	{
	case Msg_HiHoneyImHome:
		return "HiHoneyImHome";

	case Msg_StewReady:
		return "StewReady";

	case Msg_AtTheMine:
		return "AtTheMine";

	case Msg_AtTheSaloon:
		return "AtTheSaloon";

	case Msg_LetsFight:
		return "LetsFight";

	case Msg_LeavingFight:
		return "LeavingFight";

	default:
		return "Not recognized!";
	}
}

#endif
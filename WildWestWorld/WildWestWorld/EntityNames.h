#ifndef NAMES_H
#define NAMES_H

#include <string>

enum
{
	ent_Miner_Bob,
	ent_Elsa,
	ent_DrunkMiner_Marley
};

inline std::string GetNameOfEntity(int n)
{
	switch (n)
	{
	case ent_Miner_Bob:

		return "Bob";

	case ent_Elsa:

		return "Elsa";

	case ent_DrunkMiner_Marley:

		return "Marley";

	default:

		return "UNKNOWN!";
	}
}

#endif
#ifndef NAMES_H
#define NAMES_H

#include <string>

enum 
{
  ent_Miner_Bob,
  ent_DrunkMiner_Marley,
  ent_Elsa
};

inline std::string GetNameOfEntity(int n)
{
  switch(n)
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
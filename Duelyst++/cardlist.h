//Defines
#ifndef __CARDLIST_H__
#define __CARDLIST_H__

//Include
#include "unit.h"

//UnitList class
class CardList {
public:
	CardList();
	~CardList();
	std::vector<Card*> clist;
	std::vector<Unit> ulist;
};

#endif
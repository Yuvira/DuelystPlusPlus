//Include
#include "cardlist.h"

//Game constructor / destructor
CardList::CardList() {
	ulist.push_back(Unit(FACTION_LYONAR, TRIBE_GENERAL, 0, 2, 25, "resources/units/lyonargeneral1.txt"));
	clist.push_back(&ulist[0]);
}
CardList::~CardList() {}
//Include
#include "cardlist.h"

//Game constructor / destructor
CardList::CardList() {
	ulist.push_back(Unit(FACTION_LYONAR, TRIBE_GENERAL, 0, 2, 25, "lyonargeneral1", "Argeon Highmayne"));
	ulist.push_back(Unit(FACTION_NEUTRAL, TRIBE_GOLEM, 3, 4, 3, "bloodgolem", "Bloodshard Golem"));
	ulist.push_back(Unit(FACTION_NEUTRAL, TRIBE_GOLEM, 5, 4, 9, "mossgolem", "Brightmoss Golem"));
	for (int a = 0; a < ulist.size(); ++a) { clist.push_back(&ulist[a]); }
}
CardList::~CardList() {}
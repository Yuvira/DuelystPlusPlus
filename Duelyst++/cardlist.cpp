//Include
#include "card.h"

//Card list constructor / destructor
CardList::CardList() {
	glist.push_back(Unit(FACTION_LYONAR, TRIBE_GENERAL, 0, 2, 25, "lyonargeneral1", "Argeon Highmayne"));
	ulist.push_back(Unit(FACTION_NEUTRAL, TRIBE_GOLEM, 3, 4, 3, "bloodgolem", "Bloodshard Golem"));
	ulist.push_back(Unit(FACTION_NEUTRAL, TRIBE_GOLEM, 5, 4, 9, "mossgolem", "Brightmoss Golem"));
	ulist.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 4, "azureherald", "Azure Herald"));
	ulist.back().skill = el.find(SKILL_AZURE_HERALD);
	ulist.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 3, "arakiheadhunter", "Araki Headhunter"));
	ulist.back().skill = el.find(SKILL_ARAKI_HEADHUNTER);
	ulist.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 2, 2, 1, "ghostlynx", "Ghost Lynx"));
	ulist.back().skill = el.find(SKILL_GHOST_LYNX);
	ulist.push_back(Unit(FACTION_NEUTRAL, TRIBE_ARCANYST, 2, 1, 3, "aethermaster", "Aethermaster"));
	ulist.back().skill = el.find(SKILL_AETHERMASTER);
	for (int a = 0; a < glist.size(); ++a) { clist.push_back(&glist[a]); }
	for (int a = 0; a < ulist.size(); ++a) { clist.push_back(&ulist[a]); }
}
CardList::~CardList() {}

//Find card by name
Card* CardList::find(std::string s) {
	for (int a = 0; a < clist.size(); ++a) {
		if (clist[a]->name == s) {
			return clist[a];
		}
	}
	return clist[0];
}
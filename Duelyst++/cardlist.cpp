//Include
#include "card.h"

//Card list constructor / destructor
CardList::CardList() {
	glist.push_back(Unit(FACTION_LYONAR, TRIBE_GENERAL, 0, 2, 25, "lyonargeneral1", "Argeon Highmayne"));
	ulist.push_back(Unit(FACTION_NEUTRAL, TRIBE_ARCANYST, 2, 1, 3, "aethermaster", "Aethermaster"));
	ulist.back().skill = el.find(SKILL_AETHERMASTER);
	ulist.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 3, "arakiheadhunter", "Araki Headhunter"));
	ulist.back().skill = el.find(SKILL_ARAKI_HEADHUNTER);
	ulist.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 7, 7, 6, "astralcrusader", "Astral Crusader"));
	ulist.back().skill = el.find(SKILL_ASTRAL_CRUSADER);
	ulist.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 4, "azureherald", "Azure Herald"));
	ulist.back().skill = el.find(SKILL_AZURE_HERALD);
	ulist.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 4, "azureshaman", "Azure Horn Shaman"));
	ulist.back().skill = el.find(SKILL_AZURE_HORN_SHAMAN);
	ulist.push_back(Unit(FACTION_NEUTRAL, TRIBE_STRUCTURE, 3, 0, 5, "bastion", "Bastion"));
	ulist.back().skill = el.find(SKILL_BASTION);
	ulist.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 3, 4, 3, "blazehound", "Blaze Hound"));
	ulist.back().skill = el.find(SKILL_BLAZE_HOUND);
	ulist.push_back(Unit(FACTION_NEUTRAL, TRIBE_GOLEM, 3, 4, 3, "bloodgolem", "Bloodshard Golem"));
	ulist.push_back(Unit(FACTION_NEUTRAL, TRIBE_GOLEM, 5, 4, 9, "mossgolem", "Brightmoss Golem"));
	ulist.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 1, 2, 1, "dragonlark", "Dragonlark"));
	ulist.back().skill = el.find(SKILL_FLYING);
	ulist.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 2, 2, 1, "ghostlynx", "Ghost Lynx"));
	ulist.back().skill = el.find(SKILL_GHOST_LYNX);
	for (int a = 0; a < glist.size(); ++a) { clist.push_back(&glist[a]); }
	for (int a = 0; a < ulist.size(); ++a) { clist.push_back(&ulist[a]); }
	for (int a = 0; a < clist.size(); ++a) { clist[a]->original = clist[a]; }
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
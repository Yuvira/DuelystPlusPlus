//Include
#include "card.h"

//Card list constructor / destructor
CardList::CardList() {

	//Generals
	gList.push_back(Unit(FACTION_LYONAR, TRIBE_GENERAL, 0, 2, 25, "lyonargeneral1", "Argeon Highmayne"));

	//Units
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_ARCANYST, 3, 3, 1, "abjudicator", "Abjudicator"));
	uList.back().skill = el.find(SKILL_ABJUDICATOR);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_ARCANYST, 2, 1, 3, "aethermaster", "Aethermaster"));
	uList.back().skill = el.find(SKILL_AETHERMASTER);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_ARCANYST, 3, 3, 1, "loremaster", "Alcuin Loremaster"));
	uList.back().skill = el.find(SKILL_ALCUIN_LOREMASTER);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 3, "arakiheadhunter", "Araki Headhunter"));
	uList.back().skill = el.find(SKILL_ARAKI_HEADHUNTER);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 7, 7, 6, "astralcrusader", "Astral Crusader"));
	uList.back().skill = el.find(SKILL_ASTRAL_CRUSADER);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 4, "azureherald", "Azure Herald"));
	uList.back().skill = el.find(SKILL_AZURE_HERALD);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 4, "azureshaman", "Azure Horn Shaman"));
	uList.back().skill = el.find(SKILL_AZURE_HORN_SHAMAN);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_STRUCTURE, 3, 0, 5, "bastion", "Bastion"));
	uList.back().skill = el.find(SKILL_BASTION);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 3, 4, 3, "blazehound", "Blaze Hound"));
	uList.back().skill = el.find(SKILL_BLAZE_HOUND);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 4, 4, 5, "blisteringskorn", "Blistering Skorn"));
	uList.back().skill = el.find(SKILL_BLISTERING_SKORN);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_GOLEM, 3, 4, 3, "bloodgolem", "Bloodshard Golem"));
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 1, 2, 1, "bloodalchemist", "Bloodtear Alchemist"));
	uList.back().skill = el.find(SKILL_BLOODTEAR_ALCHEMIST);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 2, 3, 1, "bluetipscorpion", "Bluetip Scorpion"));
	uList.back().skill = el.find(SKILL_BLUETIP_SCORPION);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_GOLEM, 5, 4, 9, "mossgolem", "Brightmoss Golem"));
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 5, 5, 5, "chakkram", "Chakkram"));
	uList.back().skill = el.find(SKILL_CHAKKRAM);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 3, 4, 4, "chaoselemental", "Chaos Elemental"));
	uList.back().skill = el.find(SKILL_CHAOS_ELEMENTAL);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 1, 2, 1, "dragonlark", "Dragonlark"));
	uList.back().skill = el.find(SKILL_FLYING);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 2, 2, 1, "ghostlynx", "Ghost Lynx"));
	uList.back().skill = el.find(SKILL_GHOST_LYNX);
	uList.push_back(Unit(FACTION_ABYSSIAN, TRIBE_NONE, 1, 1, 1, "wraithling", "Wraithling"));

	//Spells
	sList.push_back(Spell(FACTION_ABYSSIAN, TARGET_MINION, 4, "breathofunborn", "Breath of The Unborn"));
	sList.back().spell = el.find(SPELL_BREATH_OF_THE_UNBORN);
	sList.push_back(Spell(FACTION_ABYSSIAN, TARGET_ALLY_MINON, 2, "consumingrebirth", "Consuming Rebirth"));
	sList.back().spell = el.find(SPELL_CONSUMING_REBIRTH);
	sList.push_back(Spell(FACTION_ABYSSIAN, TARGET_ENEMY_MINION, 2, "daemoniclure", "Daemonic Lure"));
	sList.back().spell = el.find(SPELL_DAEMONIC_LURE);
	sList.push_back(Spell(FACTION_ABYSSIAN, TARGET_ENEMY_GENERAL, 4, "darkseed", "Dark Seed"));
	sList.back().spell = el.find(SPELL_DARK_SEED);
	sList.push_back(Spell(FACTION_ABYSSIAN, TARGET_ENEMY_MINION, 5, "darktransformation", "Dark Transformation"));
	sList.back().spell = el.find(SPELL_DARK_TRANSFORMATION);
	sList.push_back(Spell(FACTION_ABYSSIAN, TARGET_ALLY_MINON, 0, "darkfiresacrifice", "Darkfire Sacrifice"));
	sList.back().spell = el.find(SPELL_DARKFIRE_SACRIFICE);

	//Generate card list
	for (int a = 0; a < gList.size(); ++a) { cList.push_back(&gList[a]); }
	for (int a = 0; a < uList.size(); ++a) { cList.push_back(&uList[a]); }
	for (int a = 0; a < sList.size(); ++a) { cList.push_back(&sList[a]); }

	//Set original references
	for (int a = 0; a < cList.size(); ++a) { cList[a]->original = cList[a]; }

	//Add tokens
	find("Dark Transformation")->token = find("Wraithling");

}
CardList::~CardList() {}

//Find card by name
Card* CardList::find(std::string s) {
	for (int a = 0; a < cList.size(); ++a) {
		if (cList[a]->name == s) {
			return cList[a];
		}
	}
	return cList[0];
}
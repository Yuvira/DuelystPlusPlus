//Include
#include "card.h"

//Card list constructor / destructor
CardList::CardList() {

	//Generals
	gList.push_back(Unit(FACTION_LYONAR, TRIBE_GENERAL, 0, 2, 25, "argeonhighmayne", "Argeon Highmayne"));

	//Units
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_ARCANYST, 3, 3, 1, "abjudicator", "Abjudicator"));
	uList.back().skill = el.find(SKILL_ABJUDICATOR);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_ARCANYST, 2, 1, 3, "aethermaster", "Aethermaster"));
	uList.back().skill = el.find(SKILL_AETHERMASTER);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_ARCANYST, 3, 3, 1, "alcuinloremaster", "Alcuin Loremaster"));
	uList.back().skill = el.find(SKILL_ALCUIN_LOREMASTER);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_MECH, 5, 5, 5, "alterrexx", "Alter Rexx"));
	uList.back().skill = el.find(SKILL_ALTER_REXX);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 3, "arakiheadhunter", "Araki Headhunter"));
	uList.back().skill = el.find(SKILL_ARAKI_HEADHUNTER);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_ARCANYST, 6, 7, 7, "archonspellbinder", "Archon Spellbinder"));
	uList.back().skill = el.find(SKILL_ARCHON_SPELLBINDER);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_WARMASTER, 4, 2, 4, "arrowwhistler", "Arrow Whistler"));
	uList.back().skill = el.find(SKILL_ARROW_WHISTLER);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 5, 2, 3, "ashmephyt", "Ash Mephyt"));
	uList.back().skill = el.find(SKILL_ASH_MEPHYT);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 7, 7, 6, "astralcrusader", "Astral Crusader"));
	uList.back().skill = el.find(SKILL_ASTRAL_CRUSADER);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 4, "azureherald", "Azure Herald"));
	uList.back().skill = el.find(SKILL_AZURE_HERALD);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 4, "azurehornshaman", "Azure Horn Shaman"));
	uList.back().skill = el.find(SKILL_AZURE_HORN_SHAMAN);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_STRUCTURE, 3, 0, 5, "bastion", "Bastion"));
	uList.back().skill = el.find(SKILL_BASTION);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 4, 2, 2, "blacklocust", "Black Locust"));
	uList.back().skill = el.find(SKILL_BLACK_LOCUST);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 3, 4, 3, "blazehound", "Blaze Hound"));
	uList.back().skill = el.find(SKILL_BLAZE_HOUND);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 4, 4, 5, "blisteringskorn", "Blistering Skorn"));
	uList.back().skill = el.find(SKILL_BLISTERING_SKORN);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_GOLEM, 3, 4, 3, "bloodshardgolem", "Bloodshard Golem"));
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 25, 12, 12, "bloodtaura", "Blood Taura"));
	uList.back().skill = el.find(SKILL_BLOOD_TAURA);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 1, 2, 1, "bloodtearalchemist", "Bloodtear Alchemist"));
	uList.back().skill = el.find(SKILL_BLOODTEAR_ALCHEMIST);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 2, 3, 1, "bluetipscorpion", "Bluetip Scorpion"));
	uList.back().skill = el.find(SKILL_BLUETIP_SCORPION);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 6, 2, 9, "bonereaper", "Bonereaper"));
	uList.back().skill = el.find(SKILL_BONEREAPER);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_GOLEM, 5, 4, 9, "brightmossgolem", "Brightmoss Golem"));
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 4, 2, 4, "captainhankhart", "Captain Hank Hart"));
	uList.back().skill = el.find(SKILL_CAPTAIN_HANK_HART);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 5, 5, 5, "chakkram", "Chakkram"));
	uList.back().skill = el.find(SKILL_CHAKKRAM);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 3, 4, 4, "chaoselemental", "Chaos Elemental"));
	uList.back().skill = el.find(SKILL_CHAOS_ELEMENTAL);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 3, 2, 3, "crimsonoculus", "Crimson Oculus"));
	uList.back().skill = el.find(SKILL_CRIMSON_OCULUS);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 3, 3, 3, "crossbones", "Crossbones"));
	uList.back().skill = el.find(SKILL_CROSSBONES);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 5, 2, 8, "daggerkiri", "Dagger Kiri"));
	uList.back().skill = el.find(SKILL_CELERITY);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 5, 4, 6, "dancingblades", "Dancing Blades"));
	uList.back().skill = el.find(SKILL_DANCING_BLADES);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 7, 4, 10, "darknemesis", "Dark Nemesis"));
	uList.back().skill = el.find(SKILL_DARK_NEMESIS);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 3, 3, 3, "daywatcher", "Day Watcher"));
	uList.back().skill = el.find(SKILL_DAY_WATCHER);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 6, 3, 4, "deathblighter", "Deathblighter"));
	uList.back().skill = el.find(SKILL_DEATHBLIGHTER);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 4, 4, 4, "decimus", "Decimus"));
	uList.back().skill = el.find(SKILL_DECIMUS);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_GOLEM, 6, 5, 11, "diamondgolem", "Diamond Golem"));
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 4, 5, 3, "dioltas", "Dioltas"));
	uList.back().skill = el.find(SKILL_DIOLTAS);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 1, 2, 1, "dragonlark", "Dragonlark"));
	uList.back().skill = el.find(SKILL_FLYING);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 1, 1, 1, "dreamgazer", "Dreamgazer"));
	uList.back().skill = el.find(SKILL_DREAMGAZER);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_GOLEM, 7, 10, 10, "drybonegolem", "Drybone Golem"));
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 6, 3, 4, "dustwailer", "Dust Wailer"));
	uList.back().skill = el.find(SKILL_DUST_WAILER);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_ARCANYST, 6, 3, 7, "eclipse", "Eclipse"));
	uList.back().skill = el.find(SKILL_ECLIPSE);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 4, 4, 4, "emeraldrejuvenator", "Emerald Rejuvenator"));
	uList.back().skill = el.find(SKILL_EMERALD_REJUVENATOR);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 5, 3, 10, "envybaer", "Envybaer"));
	uList.back().skill = el.find(SKILL_ENVYBAER);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 1, "ephemeralshroud", "Ephemeral Shroud"));
	uList.back().skill = el.find(SKILL_EPHEMERAL_SHROUD);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 7, 5, 5, "exun", "E'Xun"));
	uList.back().skill = el.find(SKILL_EXUN);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 6, 4, 6, "facestriker", "Facestriker"));
	uList.back().skill = el.find(SKILL_FACESTRIKER);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 5, 5, 5, "fireblazer", "Fireblazer"));
	uList.back().skill = el.find(SKILL_PROVOKE);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 4, 3, 2, "firespitter", "Fire Spitter"));
	uList.back().skill = el.find(SKILL_RANGED);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_ARCANYST, 5, 3, 5, "firestarter", "Firestarter"));
	uList.back().skill = el.find(SKILL_FIRESTARTER);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 6, 7, 7, "firstswordofakrane", "First Sword of Akrane"));
	uList.back().skill = el.find(SKILL_FIRST_SWORD_OF_AKRANE);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 2, 3, 1, "flamebloodwarlock", "Flameblood Warlock"));
	uList.back().skill = el.find(SKILL_FLAMEBLOOD_WARLOCK);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 4, 3, 3, "frostbonenaga", "Frostbone Naga"));
	uList.back().skill = el.find(SKILL_FROSTBONE_NAGA);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 2, 2, 1, "ghostlynx", "Ghost Lynx"));
	uList.back().skill = el.find(SKILL_GHOST_LYNX);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_GOLEM, 2, 2, 3, "golemmetallurgist", "Golem Metallurgist"));
	uList.back().skill = el.find(SKILL_GOLEM_METALLURGIST);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_GOLEM, 3, 2, 4, "golemvanquisher", "Golem Vanquisher"));
	uList.back().skill = el.find(SKILL_GOLEM_VANQUISHER);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 6, 5, 5, "grovelion", "Grove Lion"));
	uList.back().skill = el.find(SKILL_GROVE_LION);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_GOLEM, 4, 4, 6, "hailstonegolem", "Hailstone Golem"));
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 2, 2, 3, "healingmystic", "Healing Mystic"));
	uList.back().skill = el.find(SKILL_HEALING_MYSTIC);

	//Tokens
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 1, 1, 1, "spellspark", "Spellspark"));
	uList.back().skill = el.find(SKILL_RUSH);
	uList.push_back(Unit(FACTION_NEUTRAL, TRIBE_NONE, 3, 0, 10, "tombstone", "Tombstone"));
	uList.back().skill = el.find(SKILL_PROVOKE);
	uList.push_back(Unit(FACTION_ABYSSIAN, TRIBE_NONE, 1, 1, 1, "wraithling", "Wraithling"));

	//Spells
	sList.push_back(Spell(FACTION_ABYSSIAN, TARGET_MINION, 4, "breathoftheunborn", "Breath of The Unborn"));
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
	find("Dioltas")->token = find("Tombstone");
	find("Firestarter")->token = find("Spellspark");

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
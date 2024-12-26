//Include
#include "game.h"

//Card constructor / destructor
Card::Card() {
	faction = FACTION_NEUTRAL;
	cardType = CARD_NONE;
	isToken = false;
	cost = 0;
	game = nullptr;
	owner = nullptr;
	original = this;
	token = nullptr;
	name = "???";
	divider.CreateFromString("컴TOKEN컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴");
}
Card::~Card() {}

//Add effect to list
void Card::AddEffect(Effect effect, Card* source) {
	for (int i = 0; i < effects.size(); ++i) {
		if (effects[i].effect == effect.effect) {
			effects[i].sources.push_back(source);
			UpdateStatBuffs();
			return;
		}
	}
	effects.push_back(effect);
	effects.back().sources.push_back(source);
	UpdateStatBuffs();
}

//Remove effect from list
void Card::RemoveEffect(Effect effect, Card* source, bool allStacks) {
	for (int i = 0; i < effects.size(); ++i) {
		if (effects[i].effect == effect.effect) {
			for (int j = 0; j < effects[i].sources.size(); ++j) {
				if (effects[i].sources[j] == source) {
					effects[i].sources.erase(effects[i].sources.begin() + j);
					if (!allStacks)
						break;
					--j;
				}
			}
			if (effects[i].sources.size() == 0) {
				effects.erase(effects.begin() + i);
				break;
			}
		}
	}
	UpdateStatBuffs();
}

//Card list constructor / destructor
CardList::CardList() {

	//Generals
	generalList.push_back(Minion(FACTION_LYONAR, TRIBE_GENERAL, 0, 2, 25, "argeonhighmayne", "Argeon Highmayne"));

	//Minions
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 1, 2, 1, "dragonlark", "Dragonlark", FindEffect(SKILL_FLYING)));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 5, 5, 5, "fireblazer", "Fireblazer", FindEffect(SKILL_PROVOKE)));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 4, 3, 2, "firespitter", "Fire Spitter", FindEffect(SKILL_RANGED)));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 1, 1, 3, "komodocharger", "Komodo Charger"));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 4, 3, 2, "saberspinetiger", "Saberspine Tiger", FindEffect(SKILL_RUSH)));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 3, 2, 2, "sapphireseer", "Sapphire Seer", FindEffect(SKILL_FORCEFIELD)));

	/*

	//Units
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_ARCANYST, 3, 3, 1, "abjudicator", "Abjudicator"));
	minionList.back().skill = effectList.Find(SKILL_ABJUDICATOR);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_ARCANYST, 2, 1, 3, "aethermaster", "Aethermaster"));
	minionList.back().skill = effectList.Find(SKILL_AETHERMASTER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_ARCANYST, 3, 3, 1, "alcuinloremaster", "Alcuin Loremaster"));
	minionList.back().skill = effectList.Find(SKILL_ALCUIN_LOREMASTER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_MECH, 5, 5, 5, "alterrexx", "Alter Rexx"));
	minionList.back().skill = effectList.Find(SKILL_ALTER_REXX);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 3, "arakiheadhunter", "Araki Headhunter"));
	minionList.back().skill = effectList.Find(SKILL_ARAKI_HEADHUNTER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_ARCANYST, 6, 7, 7, "archonspellbinder", "Archon Spellbinder"));
	minionList.back().skill = effectList.Find(SKILL_ARCHON_SPELLBINDER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_WARMASTER, 4, 2, 4, "arrowwhistler", "Arrow Whistler"));
	minionList.back().skill = effectList.Find(SKILL_ARROW_WHISTLER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 5, 2, 3, "ashmephyt", "Ash Mephyt"));
	minionList.back().skill = effectList.Find(SKILL_ASH_MEPHYT);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 7, 7, 6, "astralcrusader", "Astral Crusader"));
	minionList.back().skill = effectList.Find(SKILL_ASTRAL_CRUSADER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 4, "azureherald", "Azure Herald"));
	minionList.back().skill = effectList.Find(SKILL_AZURE_HERALD);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 4, "azurehornshaman", "Azure Horn Shaman"));
	minionList.back().skill = effectList.Find(SKILL_AZURE_HORN_SHAMAN);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_STRUCTURE, 3, 0, 5, "bastion", "Bastion"));
	minionList.back().skill = effectList.Find(SKILL_BASTION);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 4, 2, 2, "blacklocust", "Black Locust"));
	minionList.back().skill = effectList.Find(SKILL_BLACK_LOCUST);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 3, 4, 3, "blazehound", "Blaze Hound"));
	minionList.back().skill = effectList.Find(SKILL_BLAZE_HOUND);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 4, 4, 5, "blisteringskorn", "Blistering Skorn"));
	minionList.back().skill = effectList.Find(SKILL_BLISTERING_SKORN);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_GOLEM, 3, 4, 3, "bloodshardgolem", "Bloodshard Golem"));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 25, 12, 12, "bloodtaura", "Blood Taura"));
	minionList.back().skill = effectList.Find(SKILL_BLOOD_TAURA);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 1, 2, 1, "bloodtearalchemist", "Bloodtear Alchemist"));
	minionList.back().skill = effectList.Find(SKILL_BLOODTEAR_ALCHEMIST);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 2, 3, 1, "bluetipscorpion", "Bluetip Scorpion"));
	minionList.back().skill = effectList.Find(SKILL_BLUETIP_SCORPION);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 6, 2, 9, "bonereaper", "Bonereaper"));
	minionList.back().skill = effectList.Find(SKILL_BONEREAPER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_GOLEM, 5, 4, 9, "brightmossgolem", "Brightmoss Golem"));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 4, 2, 4, "captainhankhart", "Captain Hank Hart"));
	minionList.back().skill = effectList.Find(SKILL_CAPTAIN_HANK_HART);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 5, 5, 5, "chakkram", "Chakkram"));
	minionList.back().skill = effectList.Find(SKILL_CHAKKRAM);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 3, 4, 4, "chaoselemental", "Chaos Elemental"));
	minionList.back().skill = effectList.Find(SKILL_CHAOS_ELEMENTAL);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 3, 2, 3, "crimsonoculus", "Crimson Oculus"));
	minionList.back().skill = effectList.Find(SKILL_CRIMSON_OCULUS);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 3, 3, 3, "crossbones", "Crossbones"));
	minionList.back().skill = effectList.Find(SKILL_CROSSBONES);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 5, 2, 8, "daggerkiri", "Dagger Kiri"));
	minionList.back().skill = effectList.Find(SKILL_CELERITY);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 5, 4, 6, "dancingblades", "Dancing Blades"));
	minionList.back().skill = effectList.Find(SKILL_DANCING_BLADES);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 7, 4, 10, "darknemesis", "Dark Nemesis"));
	minionList.back().skill = effectList.Find(SKILL_DARK_NEMESIS);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 3, 3, 3, "daywatcher", "Day Watcher"));
	minionList.back().skill = effectList.Find(SKILL_DAY_WATCHER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 6, 3, 4, "deathblighter", "Deathblighter"));
	minionList.back().skill = effectList.Find(SKILL_DEATHBLIGHTER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 4, 4, 4, "decimus", "Decimus"));
	minionList.back().skill = effectList.Find(SKILL_DECIMUS);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_GOLEM, 6, 5, 11, "diamondgolem", "Diamond Golem"));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 4, 5, 3, "dioltas", "Dioltas"));
	minionList.back().skill = effectList.Find(SKILL_DIOLTAS);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 1, 1, 1, "dreamgazer", "Dreamgazer"));
	minionList.back().skill = effectList.Find(SKILL_DREAMGAZER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_GOLEM, 7, 10, 10, "drybonegolem", "Drybone Golem"));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 6, 3, 4, "dustwailer", "Dust Wailer"));
	minionList.back().skill = effectList.Find(SKILL_DUST_WAILER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_ARCANYST, 6, 3, 7, "eclipse", "Eclipse"));
	minionList.back().skill = effectList.Find(SKILL_ECLIPSE);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 4, 4, 4, "emeraldrejuvenator", "Emerald Rejuvenator"));
	minionList.back().skill = effectList.Find(SKILL_EMERALD_REJUVENATOR);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 5, 3, 10, "envybaer", "Envybaer"));
	minionList.back().skill = effectList.Find(SKILL_ENVYBAER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 1, "ephemeralshroud", "Ephemeral Shroud"));
	minionList.back().skill = effectList.Find(SKILL_EPHEMERAL_SHROUD);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 7, 5, 5, "exun", "E'Xun"));
	minionList.back().skill = effectList.Find(SKILL_EXUN);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 6, 4, 6, "facestriker", "Facestriker"));
	minionList.back().skill = effectList.Find(SKILL_FACESTRIKER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_ARCANYST, 5, 3, 5, "firestarter", "Firestarter"));
	minionList.back().skill = effectList.Find(SKILL_FIRESTARTER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 6, 7, 7, "firstswordofakrane", "First Sword of Akrane"));
	minionList.back().skill = effectList.Find(SKILL_FIRST_SWORD_OF_AKRANE);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 2, 3, 1, "flamebloodwarlock", "Flameblood Warlock"));
	minionList.back().skill = effectList.Find(SKILL_FLAMEBLOOD_WARLOCK);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 4, 3, 3, "frostbonenaga", "Frostbone Naga"));
	minionList.back().skill = effectList.Find(SKILL_FROSTBONE_NAGA);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 2, 2, 1, "ghostlynx", "Ghost Lynx"));
	minionList.back().skill = effectList.Find(SKILL_GHOST_LYNX);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_WARMASTER, 5, 4, 6, "goldenjusticar", "Golden Justicar"));
	minionList.back().skill = effectList.Find(SKILL_GOLDEN_JUSTICAR);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_GOLEM, 2, 2, 3, "golemmetallurgist", "Golem Metallurgist"));
	minionList.back().skill = effectList.Find(SKILL_GOLEM_METALLURGIST);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_GOLEM, 3, 2, 4, "golemvanquisher", "Golem Vanquisher"));
	minionList.back().skill = effectList.Find(SKILL_GOLEM_VANQUISHER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 6, 5, 5, "grovelion", "Grove Lion"));
	minionList.back().skill = effectList.Find(SKILL_GROVE_LION);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_GOLEM, 4, 4, 6, "hailstonegolem", "Hailstone Golem"));
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 2, 2, 3, "healingmystic", "Healing Mystic"));
	minionList.back().skill = effectList.Find(SKILL_HEALING_MYSTIC);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 5, 4, 3, "ironclad", "Ironclad"));
	minionList.back().skill = effectList.Find(SKILL_IRONCLAD);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 6, 1, 1, "jaxtruesight", "Jax Truesight"));
	minionList.back().skill = effectList.Find(SKILL_JAX_TRUESIGHT);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 2, 1, 1, "jaxi", "Jaxi"));
	minionList.back().skill = effectList.Find(SKILL_JAXI);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 5, 3, 4, "keeperofthevale", "Keeper of The Vale"));
	minionList.back().skill = effectList.Find(SKILL_KEEPER_OF_THE_VALE);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 8, 5, 12, "khymera", "Khymera"));
	minionList.back().skill = effectList.Find(SKILL_KHYMERA);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 3, 2, 3, "ladylocke", "Lady Locke"));
	minionList.back().skill = effectList.Find(SKILL_LADY_LOCKE);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_ARCANYST, 4, 3, 3, "lightbender", "Lightbender"));
	minionList.back().skill = effectList.Find(SKILL_LIGHTBENDER);
	minionList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 5, 2, 5, "luxignis", "Lux Ignis"));
	minionList.back().skill = effectList.Find(SKILL_LUX_IGNIS);

	//Token units
	minionTokenList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 1, 1, 1, "minijax", "Mini-Jax"));
	minionTokenList.back().skill = effectList.Find(SKILL_RANGED);
	minionTokenList.back().isToken = true;
	minionTokenList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 1, 1, 1, "spellspark", "Spellspark"));
	minionTokenList.back().skill = effectList.Find(SKILL_RUSH);
	minionTokenList.back().isToken = true;
	minionTokenList.push_back(Minion(FACTION_NEUTRAL, TRIBE_NONE, 3, 0, 10, "tombstone", "Tombstone"));
	minionTokenList.back().skill = effectList.Find(SKILL_PROVOKE);
	minionTokenList.back().isToken = true;
	minionTokenList.push_back(Minion(FACTION_ABYSSIAN, TRIBE_NONE, 1, 1, 1, "wraithling", "Wraithling"));
	minionTokenList.back().isToken = true;

	//Spells
	spellList.push_back(Spell(FACTION_ABYSSIAN, TARGET_MINION, 4, "breathoftheunborn", "Breath of The Unborn"));
	spellList.back().spell = effectList.Find(SPELL_BREATH_OF_THE_UNBORN);
	spellList.push_back(Spell(FACTION_ABYSSIAN, TARGET_ALLY_MINON, 2, "consumingrebirth", "Consuming Rebirth"));
	spellList.back().spell = effectList.Find(SPELL_CONSUMING_REBIRTH);
	spellList.push_back(Spell(FACTION_ABYSSIAN, TARGET_ENEMY_MINION, 2, "daemoniclure", "Daemonic Lure"));
	spellList.back().spell = effectList.Find(SPELL_DAEMONIC_LURE);
	spellList.push_back(Spell(FACTION_ABYSSIAN, TARGET_ENEMY_GENERAL, 4, "darkseed", "Dark Seed"));
	spellList.back().spell = effectList.Find(SPELL_DARK_SEED);
	spellList.push_back(Spell(FACTION_ABYSSIAN, TARGET_ENEMY_MINION, 5, "darktransformation", "Dark Transformation"));
	spellList.back().spell = effectList.Find(SPELL_DARK_TRANSFORMATION);
	spellList.push_back(Spell(FACTION_ABYSSIAN, TARGET_ALLY_MINON, 0, "darkfiresacrifice", "Darkfire Sacrifice"));
	spellList.back().spell = effectList.Find(SPELL_DARKFIRE_SACRIFICE);

	//Token spells

	//Add tokens
	Find("Dark Transformation")->token = Find("Wraithling");
	Find("Dioltas")->token = Find("Tombstone");
	Find("Firestarter")->token = Find("Spellspark");
	Find("Jax Truesight")->token = Find("Mini-Jax");
	Find("Jaxi")->token = Find("Mini-Jax");

	*/

	//Set original references
	for (int i = 0; i < cardList.size(); ++i) { cardList[i]->original = cardList[i]; }

	//Generate card list
	for (int i = 0; i < generalList.size(); ++i) { cardList.push_back(&generalList[i]); }
	for (int i = 0; i < minionList.size(); ++i) { cardList.push_back(&minionList[i]); }
	for (int i = 0; i < minionTokenList.size(); ++i) { cardList.push_back(&minionTokenList[i]); }
	for (int i = 0; i < spellList.size(); ++i) { cardList.push_back(&spellList[i]); }
	for (int i = 0; i < spellTokenList.size(); ++i) { cardList.push_back(&spellTokenList[i]); }

}
CardList::~CardList() {}

//Find card by name
Card* CardList::FindCard(std::string name) {
	for (int i = 0; i < cardList.size(); ++i)
		if (cardList[i]->name == name)
			return cardList[i];
	return cardList[0];
}

//Find effect
Effect CardList::FindEffect(eEffect effect) {
	return effectList.Find(effect);
}
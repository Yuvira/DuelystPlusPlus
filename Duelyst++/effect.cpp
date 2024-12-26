//Include
#include "game.h"

#pragma region Constructors

//Effect constructors
Effect::Effect() : Effect(EFFECT_NONE, KEYWORD_NONE, 0, 0, 0, "") {}
Effect::Effect(eEffect _effect, eKeywordFlags _keywords, int _costBuff, int _atkBuff, int _hpBuff) : Effect(_effect, _keywords, _costBuff, _atkBuff, _hpBuff, "") {}
Effect::Effect(eEffect _effect, eKeywordFlags _keywords, int _costBuff, int _atkBuff, int _hpBuff, std::string description) {
	effect = _effect;
	keywords = _keywords;
	costBuff = _costBuff;
	atkBuff = _atkBuff;
	hpBuff = _hpBuff;
	if (description != "")
		GenerateSprite(description);
}
Effect::~Effect() {}

//Spell constructors
SpellEffect::SpellEffect() : SpellEffect(SPELL_NONE) {}
SpellEffect::SpellEffect(eSpell _spell) { spell = _spell; }
SpellEffect::~SpellEffect() {}

#pragma endregion

#pragma region Rendering

//Generate effect sprite
void Effect::GenerateSprite(std::string str) {

	//Split lines and get max width
	int maxWidth = 0;
	int curWidth = 0;
	std::vector<std::string> lines;
	lines.push_back("");
	for (int i = 0; i < str.length(); ++i) {
		if (str[i] == '|') {
			lines.push_back("");
			if (curWidth > maxWidth)
				maxWidth = curWidth;
			curWidth = 0;
		}
		else {
			lines.back() += str[i];
			if (str[i] != '{' && str[i] != '}')
				++curWidth;
		}
	}

	//Resize and clear
	sprite.Resize(max(maxWidth, curWidth), lines.size());

	//Draw
	int idxDelta = 0;
	WORD color = COLOR_GRAY;
	for (int i = 0; i < lines.size(); ++i) {
		for (int j = 0; j < lines[i].length(); ++j) {
			if (lines[i][j] == '{') {
				color = COLOR_LTWHITE;
				++idxDelta;
			}
			else if (lines[i][j] == '}') {
				color = COLOR_GRAY;
				++idxDelta;
			}
			else {
				sprite.buffer[(j + (i * sprite.width)) - idxDelta].Char.AsciiChar = lines[i][j];
				sprite.buffer[(j + (i * sprite.width)) - idxDelta].Attributes = color;
			}
		}
		idxDelta = 0;
	}

	/*
	std::string str = "";
	if (buffs[i].hp == 0 && buffs[i].atk != 0)
		str += (buffs[i].atk > 0 ? "+" : "-") + std::to_string(abs(buffs[i].atk)) + " Attack";
	else if (buffs[i].atk == 0 && buffs[i].hp != 0)
		str += (buffs[i].hp > 0 ? "+" : "-") + std::to_string(abs(buffs[i].hp)) + " Health";
	else if (buffs[i].atk != 0 && buffs[i].hp != 0)
		str += (buffs[i].atk > 0 ? "+" : "-") + std::to_string(buffs[i].atk) + (buffs[i].hp > 0 ? "/+" : "/-") + std::to_string(abs(buffs[i].hp));
	else if (buffs[i].cost == 0)
		str += "+0/+0";
	if (buffs[i].cost != 0) {
		if (str != "")
			str += ", ";
		str += (buffs[i].cost > 0 ? "+" : "-") + std::to_string(abs(buffs[i].cost)) + " Cost";
	}
	*/

}

//Generate spell sprites
void SpellEffect::GenerateSprite(std::string str) {
	std::vector<std::string> lines;
	lines.push_back("");
	for (int i = 0; i < str.length(); ++i) {
		if (str[i] == '|') { lines.push_back(""); }
		else { lines.back() += str[i]; }
	}
	for (int i = 0; i < lines.size(); ++i) {
		sprites.push_back(Sprite());
		sprites.back().CreateFromString(lines[i]);
		sprites.back().SetColor(COLOR_GRAY);
	}
}

#pragma endregion

#pragma region Utils

//Is skill an opening gambit
bool Effect::IsOpeningGambit() {
	switch (effect) {
	case SKILL_ABJUDICATOR:
	case SKILL_ALCUIN_LOREMASTER:
	case SKILL_ASH_MEPHYT:
	case SKILL_AZURE_HERALD:
	case SKILL_BLAZE_HOUND:
	case SKILL_BLISTERING_SKORN:
	case SKILL_BLOODTEAR_ALCHEMIST:
	case SKILL_CROSSBONES:
	case SKILL_DANCING_BLADES:
	case SKILL_DEATHBLIGHTER:
	case SKILL_DUST_WAILER:
	case SKILL_EMERALD_REJUVENATOR:
	case SKILL_EPHEMERAL_SHROUD:
	case SKILL_FLAMEBLOOD_WARLOCK:
	case SKILL_FROSTBONE_NAGA:
	case SKILL_GHOST_LYNX:
	case SKILL_HEALING_MYSTIC:
	case SKILL_JAX_TRUESIGHT:
	case SKILL_KEEPER_OF_THE_VALE:
	case SKILL_LADY_LOCKE:
	case SKILL_LIGHTBENDER:
		return true;
		break;
	}
	return false;
}

#pragma endregion

#pragma region Effect List

//Effect list constructor
EffectList::EffectList() {

	//Keyword skills
	effectList.push_back(Effect(SKILL_CELERITY, KEYWORD_CELERITY, 0, 0, 0, "{Celerity}"));
	effectList.push_back(Effect(SKILL_FLYING, KEYWORD_FLYING, 0, 0, 0, "{Flying}"));
	effectList.push_back(Effect(SKILL_FORCEFIELD, KEYWORD_FORCEFIELD, 0, 0, 0, "{Forcefield}"));
	effectList.push_back(Effect(SKILL_PROVOKE, KEYWORD_PROVOKE, 0, 0, 0, "{Provoke}"));
	effectList.push_back(Effect(SKILL_RANGED, KEYWORD_RANGED, 0, 0, 0, "{Ranged}"));
	effectList.push_back(Effect(SKILL_RUSH, KEYWORD_RUSH, 0, 0, 0, "{Rush}"));

	//Ash Mephyt
	effectList.push_back(Effect(SKILL_ASH_MEPHYT, KEYWORD_OPENING_GAMBIT, 0, 0, 0, "{Opening Gambit}: Summon two copies of|this minion on random spaces"));
	effectList.back().OnCast = [](Effect* effect, Card* card, Card* source, BoardTile* tile) {
		if (card == source && card->GetMinion() != nullptr) {
			for (int i = 0; i < 2; ++i) {
				BoardTile* newTile = card->game->map.GetRandom(tile);
				if (newTile != nullptr) {
					Minion* copy = new Minion(*(dynamic_cast<Minion*>(card->original)));
					card->game->SetContext(copy, card->owner);
					card->game->Summon(copy, newTile, false);
				}
			}
		}
	};

	/*

	//Minion skills
	skillList.push_back(Skill(SKILL_DISPELLED));
	skillList.back().GenerateSprite("{Dispelled}");
	skillList.push_back(Skill(SKILL_ABJUDICATOR));
	skillList.back().GenerateSprite("{Opening Gambit}: Lower the cost of all|spells in your action bar by 1");
	buffList.push_back(Buff(BUFF_ABJUDICATOR, -1, 0, 0, true));
	buffList.back().GenerateSprite("Abjudicator");
	skillList.push_back(Skill(SKILL_AETHERMASTER));
	skillList.back().GenerateSprite("You may replace an additional card|each turn");
	skillList.push_back(Skill(SKILL_ALCUIN_LOREMASTER));
	skillList.back().GenerateSprite("{Opening Gambit}: Put a copy of the most|recently cast spell into your|action bar");
	skillList.push_back(Skill(SKILL_ALTER_REXX));
	skillList.back().GenerateSprite("Whenever you summon MECHAZ0R, put a|MECHAZ0R in your action bar");
	skillList.push_back(Skill(SKILL_ARAKI_HEADHUNTER));
	skillList.back().GenerateSprite("Whenever you summon a minion with|Opening Gambit from your action bar,|gain +2 Attack");
	buffList.push_back(Buff(BUFF_ARAKI_HEADHUNTER, 0, 2, 0, true));
	buffList.back().GenerateSprite("Headhunter");
	skillList.push_back(Skill(SKILL_ARCHON_SPELLBINDER));
	skillList.back().GenerateSprite("Your opponent's non-Bloodborn spells|cost 1 more to cast");
	buffList.push_back(Buff(BUFF_ARCHON_SPELLBINDER, 1, 0, 0, true));
	buffList.back().GenerateSprite("Spellbound");
	skillList.push_back(Skill(SKILL_ARROW_WHISTLER));
	skillList.back().GenerateSprite("{Ranged}|Your other minions with {Ranged}|have +1 Attack");
	buffList.push_back(Buff(BUFF_ARROW_WHISTLER, 0, 1, 0, true));
	buffList.back().GenerateSprite("Whistling Arrows");
	skillList.push_back(Skill(SKILL_ASTRAL_CRUSADER));
	skillList.back().GenerateSprite("Whenever you replace this card, it|costs 3 less and gains +3/+3");
	buffList.push_back(Buff(BUFF_ASTRAL_CRUSADER, -3, 3, 3, true));
	buffList.back().GenerateSprite("Astral Crusader");
	skillList.push_back(Skill(SKILL_AZURE_HERALD));
	skillList.back().GenerateSprite("{Opening Gambit}: Restore 3 Health to|your General");
	skillList.push_back(Skill(SKILL_AZURE_HORN_SHAMAN));
	skillList.back().GenerateSprite("{Dying Wish}: Give +4 Health to friendly|minions around it");
	buffList.push_back(Buff(BUFF_AZURE_HORN_SHAMAN, 0, 0, 4, true));
	buffList.back().GenerateSprite("Azure Horn Shaman");
	skillList.push_back(Skill(SKILL_BASTION));
	skillList.back().GenerateSprite("At the end of your turn, give other|friendly minions +1 Health");
	buffList.push_back(Buff(BUFF_BASTION, 0, 0, 1, true));
	buffList.back().GenerateSprite("Bastion");
	skillList.push_back(Skill(SKILL_BLACK_LOCUST));
	skillList.back().GenerateSprite("{Flying}|After this minion moves, summon|a Black Locust nearby");
	skillList.push_back(Skill(SKILL_BLAZE_HOUND));
	skillList.back().GenerateSprite("{Opening Gambit}: Both players draw|a card");
	skillList.push_back(Skill(SKILL_BLISTERING_SKORN));
	skillList.back().GenerateSprite("{Opening Gambit}: Deal 1 damage to|everything (including itself)");
	skillList.push_back(Skill(SKILL_BLOOD_TAURA));
	skillList.back().GenerateSprite("{Provoke}|This minion's cost is equal to your|General's Health");
	buffList.push_back(Buff(BUFF_BLOOD_TAURA, -1, 0, 0, true));
	buffList.back().GenerateSprite("Blood Taura");
	skillList.push_back(Skill(SKILL_BLOODTEAR_ALCHEMIST));
	skillList.back().GenerateSprite("{Opening Gambit}: Deal 1 damage to|an enemy");
	skillList.push_back(Skill(SKILL_BLUETIP_SCORPION));
	skillList.back().GenerateSprite("Deals double damage to minions");
	skillList.push_back(Skill(SKILL_BONEREAPER));
	skillList.back().GenerateSprite("{Provoke}|At the end of your turn, deal 2 damage|to all nearby enemy minions");
	skillList.push_back(Skill(SKILL_CAPTAIN_HANK_HART));
	skillList.back().GenerateSprite("{Ranged}|Whenever this deals damage, restore|that much health to it");
	skillList.push_back(Skill(SKILL_CHAKKRAM));
	skillList.back().GenerateSprite("Costs 2 less if your General took|damage on your opponent's|last turn");
	buffList.push_back(Buff(BUFF_CHAKKRAM, -2, 0, 0, false));
	buffList.back().GenerateSprite("Chakkram");
	skillList.push_back(Skill(SKILL_CHAOS_ELEMENTAL));
	skillList.back().GenerateSprite("Whenever this minion takes damage,|it randomly teleports");
	skillList.push_back(Skill(SKILL_CRIMSON_OCULUS));
	skillList.back().GenerateSprite("Whenever your opponent summons a|minion, this gains +1/+1");
	buffList.push_back(Buff(BUFF_CRIMSON_OCULUS, 0, 1, 1, true));
	buffList.back().GenerateSprite("Crimson Oculus");
	skillList.push_back(Skill(SKILL_CROSSBONES));
	skillList.back().GenerateSprite("{Opening Gambit}: Destroy an enemy|minion with {Ranged}");
	skillList.push_back(Skill(SKILL_DANCING_BLADES));
	skillList.back().GenerateSprite("{Opening Gambit}: Deal 3 damage to|ANY minion in front of this");
	skillList.push_back(Skill(SKILL_DARK_NEMESIS));
	skillList.back().GenerateSprite("At the start of your turn, deal 4|damage to the enemy General and this|gains +4 Attack");
	buffList.push_back(Buff(BUFF_DARK_NEMESIS, 0, 4, 0, true));
	buffList.back().GenerateSprite("Dark Nemesis");
	skillList.push_back(Skill(SKILL_DAY_WATCHER));
	skillList.back().GenerateSprite("Whenever a friendly minion attacks,|restore 1 Health to your General");
	skillList.push_back(Skill(SKILL_DEATHBLIGHTER));
	skillList.back().GenerateSprite("{Opening Gambit}: Deal 3 damage to|all enemy minions around this");
	skillList.push_back(Skill(SKILL_DECIMUS));
	skillList.back().GenerateSprite("Whenever your opponent draws a card,|deal 2 damage to the enemy General");
	skillList.push_back(Skill(SKILL_DIOLTAS));
	skillList.back().GenerateSprite("{Dying Wish}: Summon a 0/8 Tombstone with|Provoke near your General");
	skillList.push_back(Skill(SKILL_DREAMGAZER));
	skillList.back().GenerateSprite("When you replace this card, summon it|nearby. Your General takes 2 damage");
	skillList.push_back(Skill(SKILL_DUST_WAILER));
	skillList.back().GenerateSprite("{Flying}|{Opening Gambit}: Deal 3 damage to all|enemies in front of this");
	skillList.push_back(Skill(SKILL_ECLIPSE));
	skillList.back().GenerateSprite("Whenever this minion takes damage, it|deals that much damage to the|enemy General");
	skillList.push_back(Skill(SKILL_EMERALD_REJUVENATOR));
	skillList.back().GenerateSprite("{Opening Gambit}: Restore 4 Health to|BOTH Generals");
	skillList.push_back(Skill(SKILL_ENVYBAER));
	skillList.back().GenerateSprite("Whenever this minion damages an enemy,|teleport that enemy to a random corner");
	skillList.push_back(Skill(SKILL_EPHEMERAL_SHROUD));
	skillList.back().GenerateSprite("{Opening Gambit}: Dispel 1 nearby space");
	skillList.push_back(Skill(SKILL_EXUN));
	skillList.back().GenerateSprite("{Forcefield}|Whenever this minion attacks or is|attacked, draw a card");
	skillList.push_back(Skill(SKILL_FACESTRIKER));
	skillList.back().GenerateSprite("Deals double damage to Generals");
	skillList.push_back(Skill(SKILL_FIRESTARTER));
	skillList.back().GenerateSprite("Whenever you cast a spell, summon a|1/1 Spellspark with {Rush} nearby");
	skillList.push_back(Skill(SKILL_FIRST_SWORD_OF_AKRANE));
	skillList.back().GenerateSprite("Your other minions have +1 Attack");
	buffList.push_back(Buff(BUFF_FIRST_SWORD_OF_AKRANE, 0, 1, 0, true));
	buffList.back().GenerateSprite("Might of Akrane");
	skillList.push_back(Skill(SKILL_FLAMEBLOOD_WARLOCK));
	skillList.back().GenerateSprite("{Opening Gambit}: Deal 3 damage to|BOTH Generals");
	skillList.push_back(Skill(SKILL_FROSTBONE_NAGA));
	skillList.back().GenerateSprite("{Opening Gambit}: Deal 2 damage to everything around this");
	skillList.push_back(Skill(SKILL_GHOST_LYNX));
	skillList.back().GenerateSprite("{Opening Gambit}: Teleport a nearby|minion to a random space");
	skillList.push_back(Skill(SKILL_GOLDEN_JUSTICAR));
	skillList.back().GenerateSprite("{Provoke}|Your other minions with {Provoke}|can move two additional spaces");
	effectList.push_back(Effect(EFFECT_GOLDEN_JUSTICAR));
	effectList.back().GenerateSprite("Golden Justicar|Can move two extra spaces");
	skillList.push_back(Skill(SKILL_GOLEM_METALLURGIST));
	skillList.back().GenerateSprite("The first Golem you summon each turn|costs 1 less");
	buffList.push_back(Buff(BUFF_GOLEM_METALLURGIST, -1, 0, 0, true));
	buffList.back().GenerateSprite("Golem Metallurgist");
	skillList.push_back(Skill(SKILL_GOLEM_VANQUISHER));
	skillList.back().GenerateSprite("{Provoke}|Your other Golem minions have {Provoke}");
	effectList.push_back(Effect(EFFECT_GOLEM_VANQUISHER));
	effectList.back().GenerateSprite("Golem Vanquisher|Provoke");
	skillList.push_back(Skill(SKILL_GROVE_LION));
	skillList.back().GenerateSprite("While this minion is on the battlefield,|your general has {Forcefield}");
	effectList.push_back(Effect(EFFECT_GROVE_LION));
	effectList.back().GenerateSprite("Grove Lion|Forcefield");
	skillList.push_back(Skill(SKILL_HEALING_MYSTIC));
	skillList.back().GenerateSprite("{Opening Gambit}: Restore 2 Health|to anything");
	skillList.push_back(Skill(SKILL_IRONCLAD));
	skillList.back().GenerateSprite("{Dying Wish}: Dispel all enemy minions");
	skillList.push_back(Skill(SKILL_JAX_TRUESIGHT));
	skillList.back().GenerateSprite("{Ranged}|{Opening Gambit}: Summon a 1/1 {Ranged}|Mini-Jax in each corner");
	skillList.push_back(Skill(SKILL_JAXI));
	skillList.back().GenerateSprite("{Dying Wish}: Summon a 1/1 {Ranged}|Mini-Jax in a random corner");
	skillList.push_back(Skill(SKILL_KEEPER_OF_THE_VALE));
	skillList.back().GenerateSprite("{Opening Gambit}: Summon a random friendly|minion destroyed this game nearby this|minion");
	skillList.push_back(Skill(SKILL_KHYMERA));
	skillList.back().GenerateSprite("Whenever this minion takes damage,|summon a random token minion nearby");
	skillList.push_back(Skill(SKILL_LADY_LOCKE));
	skillList.back().GenerateSprite("{Provoke}|{Opening Gambit}: Other minions you summon|this turn gain +1/+1 and {Provoke}");
	effectList.push_back(Effect(EFFECT_LADY_LOCKE_A));
	effectList.back().GenerateSprite("Lady Locke|Summoned minions gain +1/+1 and Provoke");
	effectList.push_back(Effect(EFFECT_LADY_LOCKE_B));
	effectList.back().GenerateSprite("Lady Locke|Provoke");
	buffList.push_back(Buff(BUFF_LADY_LOCKE, 0, 1, 1, true));
	buffList.back().GenerateSprite("Lady Locke");
	skillList.push_back(Skill(SKILL_LIGHTBENDER));
	skillList.back().GenerateSprite("{Opening Gambit}: Dispel ALL spaces|around it");
	skillList.push_back(Skill(SKILL_LUX_IGNIS));
	skillList.back().GenerateSprite("{Ranged}|At the end of your turn, restore|2 Health to all nearby friendly minions");

	//Spell effects
	spellList.push_back(SpellEffect(SPELL_BREATH_OF_THE_UNBORN));
	spellList.back().GenerateSprite("Deal 2 damage to all enemy|minions. Fully heal all friendly|minions");
	spellList.push_back(SpellEffect(SPELL_CONSUMING_REBIRTH));
	spellList.back().GenerateSprite("Destroy a friendly minion. At the end|of your turn, re-summon it on|the same space and give it +1/+1");
	buffList.push_back(Buff(BUFF_CONSUMING_REBIRTH, 0, 1, 1, true));
	buffList.back().GenerateSprite("Consuming Rebirth");
	spellList.push_back(SpellEffect(SPELL_DAEMONIC_LURE));
	spellList.back().GenerateSprite("Deal 1 damage to an enemy minion and|teleport it anywhere");
	spellList.push_back(SpellEffect(SPELL_DARK_SEED));
	spellList.back().GenerateSprite("Deal 1 damage to the enemy general|for each card in the opponent's|action bar");
	spellList.push_back(SpellEffect(SPELL_DARK_TRANSFORMATION));
	spellList.back().GenerateSprite("Destroy an enemy minion and summon|a 1/1 Wraithling on that space");
	spellList.push_back(SpellEffect(SPELL_DARKFIRE_SACRIFICE));
	spellList.back().GenerateSprite("Destroy a friendly minion. Your next|minion costs 2 less");
	effectList.push_back(Effect(EFFECT_DARKFIRE_SACRIFICE));
	effectList.back().GenerateSprite("Darkfire Sacrifice|Your next minion costs 2 less");
	buffList.push_back(Buff(BUFF_DARKFIRE_SACRIFICE, -2, 0, 0, true));
	buffList.back().GenerateSprite("Darkfire Sacrifice");

	*/

}
EffectList::~EffectList() {}

#pragma endregion

#pragma region Search Functions

//Find effect by enum
Effect EffectList::Find(eEffect effect) {
	for (int i = 0; i < effectList.size(); ++i)
		if (effectList[i].effect == effect)
			return effectList[i];
}

//Find spell by enum
SpellEffect EffectList::Find(eSpell spell) {
	for (int i = 0; i < spellList.size(); ++i)
		if (spellList[i].spell == spell)
			return spellList[i];
}

#pragma endregion
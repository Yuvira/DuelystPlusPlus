//Include
#include "effect.h"

//Skill constructor/deconstructor
Skill::Skill(eSkill s) { skill = s; }
Skill::~Skill() {}

//Effect constructor/deconstructor
Effect::Effect(eEffect e) {
	effect = e;
	count = 1;
}
Effect::~Effect() {}

//Buff constructor/deconstructor
Buff::Buff(eBuff b, int _cost, int _atk, int _hp, bool stack) {
	buff = b;
	cost = _cost;
	atk = _atk;
	hp = _hp;
	stacking = stack;
}
Buff::~Buff() {}

//Spell constructor/deconstructor
SpellEffect::SpellEffect(eSpell s) { spell = s; }
SpellEffect::~SpellEffect() {}

//Generate skill sprite
void Skill::generateSprite(std::string s) {
	std::vector<Coord> c;
	std::vector<std::string> v;
	v.push_back("");
	int tokens = 0;
	for (int a = 0; a < s.length(); ++a) {
		if (s[a] == '|') { v.push_back(""); ++tokens; }
		else if (s[a] == '{') { c.push_back(Coord()); c.back().x = a - tokens; ++tokens; }
		else if (s[a] == '}') { c.back().y = a - tokens; ++tokens; }
		else { v.back() += s[a]; }
	}
	for (int a = 0; a < v.size(); ++a) {
		sprite.push_back(Sprite());
		sprite.back().createFromString(v[a]);
		sprite.back().setCol(COLOR_GRAY);
	}
	for (int a = 0; a < c.size(); ++a) {
		for (int b = 0; b < sprite.size(); ++b) {
			if (c[a].x < sprite[b].size) {
				for (int d = c[a].x; d < c[a].y; ++d) { sprite[b].buffer[d].Attributes = COLOR_LTWHITE; }
				break;
			}
			else { 
				c[a].x -= sprite[b].size;
				c[a].y -= sprite[b].size;
			}
		}
	}
}

//Generate effect sprite
void Effect::generateSprite(std::string s) {
	std::string _s;
	for (int a = 0; a < s.length(); ++a) {
		if (s[a] == '|') {
			sprite[0].createFromString(_s);
			_s = "";
		}
		else { _s += s[a]; }
	}
	sprite[1].createFromString(_s);
	sprite[1].setCol(COLOR_GRAY);
}

//Generate buff sprite
void Buff::generateSprite(std::string s) { sprite.createFromString(s); }

//Generate spell sprite
void SpellEffect::generateSprite(std::string s) {
	std::vector<std::string> v;
	v.push_back("");
	for (int a = 0; a < s.length(); ++a) {
		if (s[a] == '|') { v.push_back(""); }
		else { v.back() += s[a]; }
	}
	for (int a = 0; a < v.size(); ++a) {
		sprite.push_back(Sprite());
		sprite.back().createFromString(v[a]);
		sprite.back().setCol(COLOR_GRAY);
	}
}

//Effect list constructor/deconstructor
EffectList::EffectList() {

	//Unit skills
	skillList.push_back(Skill(SKILL_DISPELLED));
	skillList.back().generateSprite("{Dispelled}");
	skillList.push_back(Skill(SKILL_CELERITY));
	skillList.back().generateSprite("{Celerity}");
	skillList.push_back(Skill(SKILL_FLYING));
	skillList.back().generateSprite("{Flying}");
	skillList.push_back(Skill(SKILL_PROVOKE));
	skillList.back().generateSprite("{Provoke}");
	skillList.push_back(Skill(SKILL_RANGED));
	skillList.back().generateSprite("{Ranged}");
	skillList.push_back(Skill(SKILL_RUSH));
	skillList.back().generateSprite("{Rush}");
	skillList.push_back(Skill(SKILL_ABJUDICATOR));
	skillList.back().generateSprite("{Opening Gambit}: Lower the cost of all|spells in your action bar by 1");
	buffList.push_back(Buff(BUFF_ABJUDICATOR, -1, 0, 0, true));
	buffList.back().generateSprite("Abjudicator");
	skillList.push_back(Skill(SKILL_AETHERMASTER));
	skillList.back().generateSprite("You may replace an additional card|each turn");
	skillList.push_back(Skill(SKILL_ALCUIN_LOREMASTER));
	skillList.back().generateSprite("{Opening Gambit}: Put a copy of the most|recently cast spell into your|action bar");
	skillList.push_back(Skill(SKILL_ALTER_REXX));
	skillList.back().generateSprite("Whenever you summon MECHAZ0R, put a|MECHAZ0R in your action bar");
	skillList.push_back(Skill(SKILL_ARAKI_HEADHUNTER));
	skillList.back().generateSprite("Whenever you summon a minion with|Opening Gambit from your action bar,|gain +2 Attack");
	buffList.push_back(Buff(BUFF_ARAKI_HEADHUNTER, 0, 2, 0, true));
	buffList.back().generateSprite("Headhunter");
	skillList.push_back(Skill(SKILL_ARCHON_SPELLBINDER));
	skillList.back().generateSprite("Your opponent's non-Bloodborn spells|cost 1 more to cast");
	buffList.push_back(Buff(BUFF_ARCHON_SPELLBINDER, 1, 0, 0, true));
	buffList.back().generateSprite("Spellbound");
	skillList.push_back(Skill(SKILL_ARROW_WHISTLER));
	skillList.back().generateSprite("{Ranged}|Your other minions with {Ranged}|have +1 Attack");
	buffList.push_back(Buff(BUFF_ARROW_WHISTLER, 0, 1, 0, true));
	buffList.back().generateSprite("Whistling Arrows");
	skillList.push_back(Skill(SKILL_ASH_MEPHYT));
	skillList.back().generateSprite("{Opening Gambit}: Summon two copies of|this minion on random spaces");
	skillList.push_back(Skill(SKILL_ASTRAL_CRUSADER));
	skillList.back().generateSprite("Whenever you replace this card, it|costs 3 less and gains +3/+3");
	buffList.push_back(Buff(BUFF_ASTRAL_CRUSADER, -3, 3, 3, true));
	buffList.back().generateSprite("Astral Crusader");
	skillList.push_back(Skill(SKILL_AZURE_HERALD));
	skillList.back().generateSprite("{Opening Gambit}: Restore 3 Health to|your General");
	skillList.push_back(Skill(SKILL_AZURE_HORN_SHAMAN));
	skillList.back().generateSprite("{Dying Wish}: Give +4 Health to friendly|minions around it");
	buffList.push_back(Buff(BUFF_AZURE_HORN_SHAMAN, 0, 0, 4, true));
	buffList.back().generateSprite("Azure Horn Shaman");
	skillList.push_back(Skill(SKILL_BASTION));
	skillList.back().generateSprite("At the end of your turn, give other|friendly minions +1 Health");
	buffList.push_back(Buff(BUFF_BASTION, 0, 0, 1, true));
	buffList.back().generateSprite("Bastion");
	skillList.push_back(Skill(SKILL_BLACK_LOCUST));
	skillList.back().generateSprite("{Flying}|After this minion moves, summon|a Black Locust nearby");
	skillList.push_back(Skill(SKILL_BLAZE_HOUND));
	skillList.back().generateSprite("{Opening Gambit}: Both players draw|a card");
	skillList.push_back(Skill(SKILL_BLISTERING_SKORN));
	skillList.back().generateSprite("{Opening Gambit}: Deal 1 damage to|everything (including itself)");
	skillList.push_back(Skill(SKILL_BLOOD_TAURA));
	skillList.back().generateSprite("{Provoke}|This minion's cost is equal to your|General's Health");
	buffList.push_back(Buff(BUFF_BLOOD_TAURA, -1, 0, 0, true));
	buffList.back().generateSprite("Blood Taura");
	skillList.push_back(Skill(SKILL_BLOODTEAR_ALCHEMIST));
	skillList.back().generateSprite("{Opening Gambit}: Deal 1 damage to|an enemy");
	skillList.push_back(Skill(SKILL_BLUETIP_SCORPION));
	skillList.back().generateSprite("Deals double damage to minions");
	skillList.push_back(Skill(SKILL_BONEREAPER));
	skillList.back().generateSprite("{Provoke}|At the end of your turn, deal 2 damage|to all nearby enemy minions");
	skillList.push_back(Skill(SKILL_CAPTAIN_HANK_HART));
	skillList.back().generateSprite("{Ranged}|Whenever this deals damage, restore|that much health to it");
	skillList.push_back(Skill(SKILL_CHAKKRAM));
	skillList.back().generateSprite("Costs 2 less if your General took|damage on your opponent's|last turn");
	buffList.push_back(Buff(BUFF_CHAKKRAM, -2, 0, 0, false));
	buffList.back().generateSprite("Chakkram");
	skillList.push_back(Skill(SKILL_CHAOS_ELEMENTAL));
	skillList.back().generateSprite("Whenever this minion takes damage,|it randomly teleports");
	skillList.push_back(Skill(SKILL_CRIMSON_OCULUS));
	skillList.back().generateSprite("Whenever your opponent summons a|minion, this gains +1/+1");
	buffList.push_back(Buff(BUFF_CRIMSON_OCULUS, 0, 1, 1, true));
	buffList.back().generateSprite("Crimson Oculus");
	skillList.push_back(Skill(SKILL_CROSSBONES));
	skillList.back().generateSprite("{Opening Gambit}: Destroy an enemy|minion with {Ranged}");
	skillList.push_back(Skill(SKILL_DANCING_BLADES));
	skillList.back().generateSprite("{Opening Gambit}: Deal 3 damage to|ANY minion in front of this");
	skillList.push_back(Skill(SKILL_DARK_NEMESIS));
	skillList.back().generateSprite("At the start of your turn, deal 4|damage to the enemy General and this|gains +4 Attack");
	buffList.push_back(Buff(BUFF_DARK_NEMESIS, 0, 4, 0, true));
	buffList.back().generateSprite("Dark Nemesis");
	skillList.push_back(Skill(SKILL_DAY_WATCHER));
	skillList.back().generateSprite("Whenever a friendly minion attacks,|restore 1 Health to your General");
	skillList.push_back(Skill(SKILL_DEATHBLIGHTER));
	skillList.back().generateSprite("{Opening Gambit}: Deal 3 damage to|all enemy minions around this");
	skillList.push_back(Skill(SKILL_DECIMUS));
	skillList.back().generateSprite("Whenever your opponent draws a card,|deal 2 damage to the enemy General");
	skillList.push_back(Skill(SKILL_DIOLTAS));
	skillList.back().generateSprite("{Dying Wish}: Summon a 0/8 Tombstone with|Provoke near your General");
	skillList.push_back(Skill(SKILL_DREAMGAZER));
	skillList.back().generateSprite("When you replace this card, summon it|nearby. Your General takes 2 damage");
	skillList.push_back(Skill(SKILL_DUST_WAILER));
	skillList.back().generateSprite("{Flying}|{Opening Gambit}: Deal 3 damage to all|enemies in front of this");
	skillList.push_back(Skill(SKILL_ECLIPSE));
	skillList.back().generateSprite("Whenever this minion takes damage, it|deals that much damage to the|enemy General");
	skillList.push_back(Skill(SKILL_EMERALD_REJUVENATOR));
	skillList.back().generateSprite("{Opening Gambit}: Restore 4 Health to|BOTH Generals");
	skillList.push_back(Skill(SKILL_ENVYBAER));
	skillList.back().generateSprite("Whenever this minion damages an enemy,|teleport that enemy to a random corner");
	skillList.push_back(Skill(SKILL_EPHEMERAL_SHROUD));
	skillList.back().generateSprite("{Opening Gambit}: Dispel 1 nearby space");
	skillList.push_back(Skill(SKILL_EXUN));
	skillList.back().generateSprite("{Forcefield}|Whenever this minion attacks or is|attacked, draw a card");
	skillList.push_back(Skill(SKILL_FACESTRIKER));
	skillList.back().generateSprite("Deals double damage to Generals");
	skillList.push_back(Skill(SKILL_FIRESTARTER));
	skillList.back().generateSprite("Whenever you cast a spell, summon a|1/1 Spellspark with {Rush} nearby");
	skillList.push_back(Skill(SKILL_FIRST_SWORD_OF_AKRANE));
	skillList.back().generateSprite("Your other minions have +1 Attack");
	buffList.push_back(Buff(BUFF_FIRST_SWORD_OF_AKRANE, 0, 1, 0, true));
	buffList.back().generateSprite("Might of Akrane");
	skillList.push_back(Skill(SKILL_FLAMEBLOOD_WARLOCK));
	skillList.back().generateSprite("{Opening Gambit}: Deal 3 damage to|BOTH Generals");
	skillList.push_back(Skill(SKILL_FROSTBONE_NAGA));
	skillList.back().generateSprite("{Opening Gambit}: Deal 2 damage to everything around this");
	skillList.push_back(Skill(SKILL_GHOST_LYNX));
	skillList.back().generateSprite("{Opening Gambit}: Teleport a nearby|minion to a random space");
	skillList.push_back(Skill(SKILL_GOLDEN_JUSTICAR));
	skillList.back().generateSprite("{Provoke}|Your other minions with {Provoke}|can move two additional spaces");
	effectList.push_back(Effect(EFFECT_GOLDEN_JUSTICAR));
	effectList.back().generateSprite("Golden Justicar|Can move two extra spaces");
	skillList.push_back(Skill(SKILL_GOLEM_METALLURGIST));
	skillList.back().generateSprite("The first Golem you summon each turn|costs 1 less");
	buffList.push_back(Buff(BUFF_GOLEM_METALLURGIST, -1, 0, 0, true));
	buffList.back().generateSprite("Golem Metallurgist");
	skillList.push_back(Skill(SKILL_GOLEM_VANQUISHER));
	skillList.back().generateSprite("{Provoke}|Your other Golem minions have {Provoke}");
	effectList.push_back(Effect(EFFECT_GOLEM_VANQUISHER));
	effectList.back().generateSprite("Golem Vanquisher|Provoke");
	skillList.push_back(Skill(SKILL_GROVE_LION));
	skillList.back().generateSprite("While this minion is on the battlefield,|your general has {Forcefield}");
	effectList.push_back(Effect(EFFECT_GROVE_LION));
	effectList.back().generateSprite("Grove Lion|Forcefield");
	skillList.push_back(Skill(SKILL_HEALING_MYSTIC));
	skillList.back().generateSprite("{Opening Gambit}: Restore 2 Health|to anything");
	skillList.push_back(Skill(SKILL_IRONCLAD));
	skillList.back().generateSprite("{Dying Wish}: Dispel all enemy minions");
	skillList.push_back(Skill(SKILL_JAX_TRUESIGHT));
	skillList.back().generateSprite("{Ranged}|{Opening Gambit}: Summon a 1/1 {Ranged}|Mini-Jax in each corner");
	skillList.push_back(Skill(SKILL_JAXI));
	skillList.back().generateSprite("{Dying Wish}: Summon a 1/1 {Ranged}|Mini-Jax in a random corner");
	skillList.push_back(Skill(SKILL_KEEPER_OF_THE_VALE));
	skillList.back().generateSprite("{Opening Gambit}: Summon a random friendly|minion destroyed this game nearby this|minion");
	skillList.push_back(Skill(SKILL_KHYMERA));
	skillList.back().generateSprite("Whenever this minion takes damage,|summon a random token minion nearby");

	//Spell effects
	spellList.push_back(SpellEffect(SPELL_BREATH_OF_THE_UNBORN));
	spellList.back().generateSprite("Deal 2 damage to all enemy|minions. Fully heal all friendly|minions");
	spellList.push_back(SpellEffect(SPELL_CONSUMING_REBIRTH));
	spellList.back().generateSprite("Destroy a friendly minion. At the end|of your turn, re-summon it on|the same space and give it +1/+1");
	buffList.push_back(Buff(BUFF_CONSUMING_REBIRTH, 0, 1, 1, true));
	buffList.back().generateSprite("Consuming Rebirth");
	spellList.push_back(SpellEffect(SPELL_DAEMONIC_LURE));
	spellList.back().generateSprite("Deal 1 damage to an enemy minion and|teleport it anywhere");
	spellList.push_back(SpellEffect(SPELL_DARK_SEED));
	spellList.back().generateSprite("Deal 1 damage to the enemy general|for each card in the opponent's|action bar");
	spellList.push_back(SpellEffect(SPELL_DARK_TRANSFORMATION));
	spellList.back().generateSprite("Destroy an enemy minion and summon|a 1/1 Wraithling on that space");
	spellList.push_back(SpellEffect(SPELL_DARKFIRE_SACRIFICE));
	spellList.back().generateSprite("Destroy a friendly minion. Your next|minion costs 2 less");
	effectList.push_back(Effect(EFFECT_DARKFIRE_SACRIFICE));
	effectList.back().generateSprite("Darkfire Sacrifice|Your next minion costs 2 less");
	buffList.push_back(Buff(BUFF_DARKFIRE_SACRIFICE, -2, 0, 0, true));
	buffList.back().generateSprite("Darkfire Sacrifice");

}
EffectList::~EffectList() {}

//Find skill by enum
Skill EffectList::find(eSkill s) {
	for (int a = 0; a < skillList.size(); ++a) {
		if (skillList[a].skill == s) {
			return skillList[a];
		}
	}
}

//Find effect by enum
Effect EffectList::find(eEffect e) {
	for (int a = 0; a < effectList.size(); ++a) {
		if (effectList[a].effect == e) {
			return effectList[a];
		}
	}
}

//Find buff by enum
Buff EffectList::find(eBuff b) {
	for (int a = 0; a < buffList.size(); ++a) {
		if (buffList[a].buff == b) {
			return buffList[a];
		}
	}
}

//Find spell by enum
SpellEffect EffectList::find(eSpell s) {
	for (int a = 0; a < spellList.size(); ++a) {
		if (spellList[a].spell == s) {
			return spellList[a];
		}
	}
}
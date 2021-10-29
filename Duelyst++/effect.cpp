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
	for (int a = 0; a < s.length(); ++a) {
		if (s[a] == '|') { v.push_back(""); }
		else if (s[a] == '{') { c.push_back(Coord()); c.back().x = a; }
		else if (s[a] == '}') { c.back().y = a - 1; }
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
	skillList.push_back(Skill(SKILL_FLYING));
	skillList.back().generateSprite("{Flying}");
	skillList.push_back(Skill(SKILL_AETHERMASTER));
	skillList.back().generateSprite("You may replace an additional card|each turn");
	effectList.push_back(Effect(EFFECT_AETHERMASTER));
	effectList.back().generateSprite("Aethermaster|You may replace 1 extra card(s) per turn");
	skillList.push_back(Skill(SKILL_ARAKI_HEADHUNTER));
	skillList.back().generateSprite("Whenever you summon a minion with|Opening Gambit from your action bar,|gain +2 Attack");
	buffList.push_back(Buff(BUFF_ARAKI_HEADHUNTER, 0, 2, 0, true));
	buffList.back().generateSprite("Headhunter");
	skillList.push_back(Skill(SKILL_ASTRAL_CRUSADER));
	skillList.back().generateSprite("Whenever you replace this card, it|costs 3 less and gains +3/+3");
	buffList.push_back(Buff(BUFF_ASTRAL_CRUSADER, -3, 3, 3, true));
	buffList.back().generateSprite("Astral Crusader");
	skillList.push_back(Skill(SKILL_AZURE_HERALD));
	skillList.back().generateSprite("{Opening Gambit}: Restore 3 Health to your|General");
	skillList.push_back(Skill(SKILL_AZURE_HORN_SHAMAN));
	skillList.back().generateSprite("{Dying Wish}: Give +4 Health to friendly|minions around it");
	buffList.push_back(Buff(BUFF_AZURE_HORN_SHAMAN, 0, 0, 4, true));
	buffList.back().generateSprite("Azure Horn Shaman");
	skillList.push_back(Skill(SKILL_BASTION));
	skillList.back().generateSprite("At the end of your turn, give other|friendly minions +1 Health");
	buffList.push_back(Buff(BUFF_BASTION, 0, 0, 1, true));
	buffList.back().generateSprite("Bastion");
	skillList.push_back(Skill(SKILL_BLAZE_HOUND));
	skillList.back().generateSprite("{Opening Gambit}: Both players draw|a card");
	skillList.push_back(Skill(SKILL_BLISTERING_SKORN));
	skillList.back().generateSprite("{Opening Gambit}: Deal 1 damage to|everything (including itself)");
	skillList.push_back(Skill(SKILL_BLOODTEAR_ALCHEMIST));
	skillList.back().generateSprite("{Opening Gambit}: Deal 1 damage to|an enemy");
	skillList.push_back(Skill(SKILL_BLUETIP_SCORPION));
	skillList.back().generateSprite("Deals double damage to minions");
	skillList.push_back(Skill(SKILL_CHAKKRAM));
	skillList.back().generateSprite("Costs 2 less if your General took|damage on your opponent's|last turn");
	buffList.push_back(Buff(BUFF_CHAKKRAM, -2, 0, 0, false));
	buffList.back().generateSprite("Chakkram");
	skillList.push_back(Skill(SKILL_CHAOS_ELEMENTAL));
	skillList.back().generateSprite("Whenever this minion takes damage,|it randomly teleports");
	skillList.push_back(Skill(SKILL_GHOST_LYNX));
	skillList.back().generateSprite("{Opening Gambit}: Teleport a nearby|minion to a random space");

	//Spell effects
	spellList.push_back(SpellEffect(SPELL_BREATH_OF_THE_UNBORN));
	spellList.back().generateSprite("Deal 2 damage to all enemy|minions. Fully heal all friendly|minions");
	spellList.push_back(SpellEffect(SPELL_CONSUMING_REBIRTH));
	spellList.back().generateSprite("Destroy a friendly minion. At the end|of your turn, re-summon it on|the same space and give it +1/+1");
	buffList.push_back(Buff(BUFF_CONSUMING_REBIRTH, 0, 1, 1, true));
	buffList.back().generateSprite("Consuming Rebirth");

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
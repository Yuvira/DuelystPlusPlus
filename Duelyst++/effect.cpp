//Include
#include "effect.h"

//Effect constructor/deconstructor
Skill::Skill(eSkill s) { skill = s; }
Skill::~Skill() {}

//Effect constructor/deconstructor
Effect::Effect(eEffect e) {
	effect = e;
	count = 1;
}
Effect::~Effect() {}

//Buff constructor/deconstructor
Buff::Buff(eBuff b, int _cost, int _atk, int _hp) {
	buff = b;
	cost = _cost;
	atk = _atk;
	hp = _hp;
}
Buff::~Buff() {}

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

//Effect list constructor/deconstructor
EffectList::EffectList() {
	slist.push_back(Skill(SKILL_FLYING));
	slist.back().generateSprite("{Flying}");
	slist.push_back(Skill(SKILL_AETHERMASTER));
	slist.back().generateSprite("You may replace an additional card|each turn");
	elist.push_back(Effect(EFFECT_AETHERMASTER));
	elist.back().generateSprite("Aethermaster|You may replace 1 extra card(s) per turn");
	slist.push_back(Skill(SKILL_ARAKI_HEADHUNTER));
	slist.back().generateSprite("Whenever you summon a minion with|Opening Gambit from your action bar,|gain +2 Attack");
	blist.push_back(Buff(BUFF_ARAKI_HEADHUNTER, 0, 2, 0));
	blist.back().generateSprite("Headhunter");
	slist.push_back(Skill(SKILL_ASTRAL_CRUSADER));
	slist.back().generateSprite("Whenever you replace this card, it|costs 3 less and gains +3/+3");
	blist.push_back(Buff(BUFF_ASTRAL_CRUSADER, -3, 3, 3));
	blist.back().generateSprite("Astral Crusader");
	slist.push_back(Skill(SKILL_AZURE_HERALD));
	slist.back().generateSprite("{Opening Gambit}: Restore 3 Health to your|General");
	slist.push_back(Skill(SKILL_AZURE_HORN_SHAMAN));
	slist.back().generateSprite("{Dying Wish}: Give +4 Health to friendly|minions around it");
	blist.push_back(Buff(BUFF_AZURE_HORN_SHAMAN, 0, 0, 4));
	blist.back().generateSprite("Azure Horn Shaman");
	slist.push_back(Skill(SKILL_BASTION));
	slist.back().generateSprite("At the end of your turn, give other|friendly minions +1 Health");
	blist.push_back(Buff(BUFF_BASTION, 0, 0, 1));
	blist.back().generateSprite("Bastion");
	slist.push_back(Skill(SKILL_BLAZE_HOUND));
	slist.back().generateSprite("{Opening Gambit}: Both players draw|a card");
	slist.push_back(Skill(SKILL_BLISTERING_SKORN));
	slist.back().generateSprite("{Opening Gambit}: Deal 1 damage to|everything (including itself)");
	slist.push_back(Skill(SKILL_GHOST_LYNX));
	slist.back().generateSprite("{Opening Gambit}: Teleport a nearby|minion to a random space");
}
EffectList::~EffectList() {}

//Find skill by enum
Skill EffectList::find(eSkill s) {
	for (int a = 0; a < slist.size(); ++a) {
		if (slist[a].skill == s) {
			return slist[a];
		}
	}
}

//Find effect by enum
Effect EffectList::find(eEffect e) {
	for (int a = 0; a < elist.size(); ++a) {
		if (elist[a].effect == e) {
			return elist[a];
		}
	}
}

//Find buff by enum
Buff EffectList::find(eBuff b) {
	for (int a = 0; a < blist.size(); ++a) {
		if (blist[a].buff == b) {
			return blist[a];
		}
	}
}
//Include
#include "effect.h"

//Effect constructor/deconstructor
Effect::Effect(eEffect e) { effect = e; }
Effect::~Effect() {}

//Buff constructor/deconstructor
Buff::Buff(eBuff b, int _atk, int _hp) {
	buff = b;
	atk = _atk;
	hp = _hp;
}
Buff::~Buff() {}

//Generate effect sprite
void Effect::generateSprite(std::string s) {
	std::vector<Coord> c;
	std::vector<std::string> v;
	v.push_back("");
	for (int a = 0; a < s.length(); ++a) {
		if (s[a] == '|') { v.push_back(""); }
		else if (s[a] == '{') { c.push_back(Coord()); c.back().x = a; }
		else if (s[a] == '}') { c.back().y = a; }
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

//Generate buff sprite
void Buff::generateSprite(std::string s) { sprite.createFromString(s); }

//Effect list constructor/deconstructor
EffectList::EffectList() {
	elist.push_back(Effect(EFFECT_AZURE_HERALD));
	elist.back().generateSprite("{Opening Gambit}: Restore 3 Health to your|General.");
	elist.push_back(Effect(EFFECT_ARAKI_HEADHUNTER));
	elist.back().generateSprite("Whenever you summon a minion with|Opening Gambit from your action bar,|gain +2 Attack.");
	blist.push_back(Buff(BUFF_ARAKI_HEADHUNTER, 2, 0));
	blist.back().generateSprite("Headhunter");
}
EffectList::~EffectList() {}

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
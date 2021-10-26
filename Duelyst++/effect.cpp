//Include
#include "effect.h"

//Effect constructor/deconstructor
Effect::Effect(eEffect e) { effect = e; }
Effect::~Effect() {}

//Buff constructor/deconstructor
Buff::Buff(eBuff b) {
	buff = b;
	atk = 0;
	hp = 0;
	count = 0;
}
Buff::~Buff() {}

//Generate effect sprite
void Effect::generateSprite(std::string s) {
	std::vector<Coord> c;
	std::vector<std::string> v;
	v.push_back("");
	for (int a = 0; a < s.size(); ++a) {
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

//Effect list constructor/deconstructor
EffectList::EffectList() {
	elist.push_back(Effect(EFFECT_AZURE_HERALD));
	elist[0].generateSprite("{Opening Gambit}: Restore 3 Health to your|General.");
}
EffectList::~EffectList() {}
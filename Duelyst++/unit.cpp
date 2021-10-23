//Include
#include "unit.h"

//Game constructor / destructor
Unit::Unit(eFaction _faction, eTribe _tribe, int _cost, int _attack, int _health, std::string path) {
	type = CARD_UNIT;
	faction = _faction;
	tribe = _tribe;
	cost = _cost;
	attack = _attack;
	health = _health;
	onSummon = std::bind(&Unit::doNothing, this);
	onSummonAny = std::bind(&Unit::doNothingU, this, std::placeholders::_1);
	onDeath = std::bind(&Unit::doNothing, this);
	onDeathAny = std::bind(&Unit::doNothingU, this, std::placeholders::_1);
	if (path == "") { sprite.resize(5, 5); }
	else { sprite.createFromFile(path); }
	updateStatSprites();
}
Unit::~Unit() {}

//Render sprite
void Unit::render(Renderer& rm) {
	rm.render(sprite);
	rm.render(sATK);
	rm.render(sHP);
}

//Set sprite position
void Unit::setPos(int x, int y, Map& m) {
	for (int a = 0; a < 9; ++a) {
		for (int b = 0; b < 5; ++b) {
			if (m.tile[a][b].unit == this) {
				m.tile[a][b].unit = nullptr;
				break;
			}
		}
	}
	int _x = (x * 6) + 2;
	int _y = (y * 6) + 2;
	sprite.pos.X = _x;
	sprite.pos.Y = _y;
	sATK.pos.X = _x;
	sATK.pos.Y = _y + 4;
	sHP.pos.X = (_x + 4) - (sHP.size - 1);
	sHP.pos.Y = _y + 4;
	m.tile[x][y].unit = this;
}

//Update HP & ATK sprites
void Unit::updateStatSprites() {
	std::string s = std::to_string(attack);
	sATK.resize(s.length(), 1);
	for (int a = 0; a < s.length(); ++a) {
		sATK.buffer[a].Char.AsciiChar = s[a];
		sATK.buffer[a].Attributes = FOREGROUND_GREEN;
	}
	s = std::to_string(health);
	sHP.resize(s.length(), 1);
	for (int a = 0; a < s.length(); ++a) {
		sHP.buffer[a].Char.AsciiChar = s[a];
		sHP.buffer[a].Attributes = FOREGROUND_RED;
	}
}

//Do nothing
void Unit::doNothing() {}
void Unit::doNothingU(Unit& u) {}
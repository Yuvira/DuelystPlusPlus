//Include
#include "unit.h"

//Game constructor / destructor
Unit::Unit(eFaction _faction, eTribe _tribe, int _cost, int _atk, int _hp, std::string path, std::string _name) {
	type = CARD_UNIT;
	faction = _faction;
	tribe = _tribe;
	cost = _cost;
	atk = _atk;
	hp = _hp;
	name = _name;
	onSummon = std::bind(&Unit::doNothing, this);
	onSummonAny = std::bind(&Unit::doNothingU, this, std::placeholders::_1);
	onDeath = std::bind(&Unit::doNothing, this);
	onDeathAny = std::bind(&Unit::doNothingU, this, std::placeholders::_1);
	onAttack = std::bind(&Unit::doNothingU, this, std::placeholders::_1);
	onAttacked = std::bind(&Unit::doNothingU, this, std::placeholders::_1);
	if (path == "") { sprite.resize(5, 5); }
	else { sprite.createFromFile("resources/units/" + path + ".txt"); }
	updateStatSprites();
}
Unit::~Unit() {}

//Render sprite
void Unit::render(Renderer& rm) {
	rm.render(sprite);
	rm.render(sATK);
	rm.render(sHP);
}

//Attack enemy
void Unit::attack(Unit& u) {
	u.hp -= atk;
	onAttack(u);
	u.onAttacked(*this);
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
	int _x = (x * 7) + 2;
	int _y = (y * 7) + 2;
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
	std::string s = std::to_string(atk);
	sATK.resize(s.length(), 1);
	for (int a = 0; a < s.length(); ++a) {
		sATK.buffer[a].Char.AsciiChar = s[a];
		sATK.buffer[a].Attributes = COLOR_GREEN;
	}
	int i = sHP.size;
	s = std::to_string(hp);
	sHP.resize(s.length(), 1);
	for (int a = 0; a < s.length(); ++a) {
		sHP.buffer[a].Char.AsciiChar = s[a];
		sHP.buffer[a].Attributes = COLOR_RED;
	}
	sHP.pos.X += i - s.length();
}

//Do nothing
void Unit::doNothing() {}
void Unit::doNothingU(Unit& u) {}
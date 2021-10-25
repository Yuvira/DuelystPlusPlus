//Include
#include "game.h"

//Game constructor / destructor
Unit::Unit(eFaction _faction, eTribe _tribe, int _cost, int _atk, int _hp, std::string path, std::string _name) {
	type = CARD_UNIT;
	faction = _faction;
	tribe = _tribe;
	cost = _cost;
	atk = _atk;
	hp = _hp;
	name = _name;
	if (path == "") { sprite.resize(5, 5); }
	else { sprite.createFromFile("resources/units/" + path + ".txt"); }
	updateStatSprites();
	generateDetails();
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
	int _y = (y * 7) + 6;
	sprite.pos.X = _x;
	sprite.pos.Y = _y;
	sATK.pos.X = _x;
	sATK.pos.Y = _y + 4;
	sHP.pos.X = (_x + 4) - (sHP.size - 1);
	sHP.pos.Y = _y + 4;
	m.tile[x][y].unit = this;
}

//Check if unit has died
void Unit::update(bool& r) {
	if (hp < 1) {
		onDeath();
		for (int a = 0; a < game->unit.size(); ++a) { game->unit[a]->onDeathAny(*this); }
		r = true;
	}
}

//Update HP & ATK sprites
void Unit::updateStatSprites() {
	std::string s = std::to_string(atk);
	sATK.resize(s.length(), 1);
	for (int a = 0; a < s.length(); ++a) { sATK.buffer[a].Char.AsciiChar = s[a]; }
	sATK.setCol(COLOR_GREEN);
	int i = sHP.size;
	s = std::to_string(hp);
	sHP.resize(s.length(), 1);
	for (int a = 0; a < s.length(); ++a) { sHP.buffer[a].Char.AsciiChar = s[a]; }
	sHP.setCol(COLOR_RED);
	sHP.pos.X += i - s.length();
}

//Update card stats
void Unit::updateDetailStats() {
	eColor col = COLOR_LTBLUE;
	std::string s = "";
	if (tribe != TRIBE_GENERAL) { s = "COST:" + std::to_string(cost) + " ATK:" + std::to_string(atk) + " HP:" + std::to_string(hp); }
	else { s = "ATK:" + std::to_string(atk) + " HP:" + std::to_string(hp); col = COLOR_GREEN; }
	description[1].createFromString(s);
	for (int a = 0; a < description[1].size; ++a) {
		if (s[a] != ':' && s[a] != ' ') { description[1].buffer[a].Attributes = col; }
		if (s[a] == ' ') {
			if (col == COLOR_LTBLUE) { col = COLOR_GREEN; }
			else { col = COLOR_RED; }
		}
	}
}

//Generate sidebar details
void Unit::generateDetails() {
	std::string s;
	Sprite sprite;
	s.resize(name.size());
	std::transform(name.begin(), name.end(), s.begin(), ::toupper);
	s += " - ";
	switch (tribe) {
		case TRIBE_NONE:
			s += "MINION";
			break;
		case TRIBE_GENERAL:
			s += "GENERAL";
			break;
		case TRIBE_ARCANYST:
			s += "ARCANYST";
			break;
		case TRIBE_PET:
			s += "PET";
			break;
		case TRIBE_GOLEM:
			s += "GOLEM";
			break;
		case TRIBE_MECH:
			s += "MECH";
			break;
		case TRIBE_DERVISH:
			s += "DERVISH";
			break;
		case TRIBE_VESPYR:
			s += "VESPYR";
			break;
		case TRIBE_STRUCTURE:
			s += "STRUCTURE";
			break;
	}
	sprite.createFromString(s);
	for (int a = name.size(); a < sprite.size; ++a) { sprite.buffer[a].Attributes = COLOR_GRAY; }
	description.push_back(sprite);
	description.push_back(Sprite());
	updateDetailStats();
}

//Draw card data
void Unit::drawCard(Renderer& rm, int& y) {
	rm.render(sprite, 66, y);
	rm.render(description[0], 72, y); ++y;
	updateDetailStats();
	rm.render(description[1], 72, y); ++y;
}

//When this unit is summoned (Opening Gambit)
void Unit::onSummon() {}

//When any unit is summoned
void Unit::onSummonAny(Unit& u) {}

//When this unit dies
void Unit::onDeath() {

	//Remove tile reference
	for (int a = 0; a < 9; ++a) {
		for (int b = 0; b < 5; ++b) {
			if (game->map.tile[a][b].unit == this) {
				game->map.tile[a][b].unit = nullptr;
				for (int c = 0; c < game->hostile.size(); ++c) {
					if (game->hostile[c] == &game->map.tile[a][b]) {
						game->hostile.erase(game->hostile.begin() + c);
						break;
					}
				}
				goto end;
			}
		}
	}
	end:

	//Move to graveyard
	for (int a = 0; a < game->unit.size(); ++a) {
		if (game->unit[a] == this) {
			game->unit.erase(game->unit.begin() + a);
			game->grave.push_back(this);
			break;
		}
	}

}

//When any unit dies (Deathwatch)
void Unit::onDeathAny(Unit& u) {}

//When this unit attacks
void Unit::onAttack(Unit& u) {}

//When this unit is attacked
void Unit::onAttacked(Unit& u) {}
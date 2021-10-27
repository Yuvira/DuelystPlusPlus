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
	hpMax = _hp;
	name = _name;
	moved = false;
	attacked = false;
	if (path == "") { sprite.resize(5, 5); }
	else { sprite.createFromFile("resources/units/" + path + ".txt"); }
	updateStatSprites();
	generateDetails();
	tile = nullptr;
	game = nullptr;
	player = nullptr;
}
Unit::~Unit() {}

//Render sprite
void Unit::render(Renderer& rm) {
	attacked ? sprite.setCol(COLOR_GRAY) : sprite.setCol(COLOR_LTWHITE);
	rm.render(sprite);
	rm.render(sATK);
	rm.render(sHP);
}

//Draw card data
void Unit::drawDetails(Renderer& rm, int& y) {
	sprite.setCol(COLOR_LTWHITE);
	rm.render(sprite, 66, y);
	rm.render(header[0], 72, y); ++y;
	updateDetailStats();
	rm.render(header[1], 72, y); y+= 2;
	for (int a = 0; a < skill.sprite.size(); ++a) {
		rm.render(skill.sprite[a], 72, y);
		if (a == skill.sprite.size() - 1) { ++y; }
		++y;
	}
	for (int a = 0; a < effect.size(); ++a) {
		switch (effect[a].effect) {
		case EFFECT_AETHERMASTER:
			effect[a].sprite[1].buffer[16].Char.AsciiChar = std::to_string(effect[a].count)[0];
			break;
		}
		rm.render(effect[a].sprite[0], 72, y);
		rm.render(effect[a].sprite[1], 72, y + 1);
		y += 3;
	}
	for (int a = 0; a < buff.size(); ++a) {
		rm.render(buff[a].sprite, 72, y); ++y;
		Sprite s = Sprite();
		if (buff[a].hp == 0 && buff[a].atk != 0) { s.createFromString((buff[a].atk > 0 ? "+" : "-") + std::to_string(abs(buff[a].atk)) + " Attack"); }
		else if (buff[a].atk == 0 && buff[a].hp != 0) { s.createFromString((buff[a].hp > 0 ? "+" : "-") + std::to_string(abs(buff[a].hp)) + " Health"); }
		else if (buff[a].atk != 0 && buff[a].hp != 0) { s.createFromString((buff[a].atk > 0 ? "+" : "-") + std::to_string(buff[a].atk) + (buff[a].hp > 0 ? "+" : "-") + std::to_string(abs(buff[a].hp))); }
		else { s.createFromString("+0/+0"); }
		s.setCol(COLOR_GRAY);
		rm.render(s, 72, y); y += 2;
	}
}

//Attack enemy
void Unit::attack(Unit& u, bool counter) {
	u.hp -= atk;
	if (!counter) {
		moved = true;
		attacked = true;
	}
}

//Set sprite position
void Unit::setPos(int x, int y) {
	if (tile != nullptr) { tile->unit = nullptr; }
	int _x = (x * 7) + 2;
	int _y = (y * 7) + 6;
	sprite.pos.X = _x;
	sprite.pos.Y = _y;
	sATK.pos.X = _x;
	sATK.pos.Y = _y + 4;
	sHP.pos.X = (_x + 4) - (sHP.size - 1);
	sHP.pos.Y = _y + 4;
	tile = &game->map.tile[x][y];
	tile->unit = this;
}

//Add buff to list
void Unit::addBuff(eBuff b) {
	Buff _b = game->cl.el.find(b);
	for (int a = 0; a < buff.size(); ++a) {
		if (buff[a].buff == b) {
			buff[a].atk += _b.atk;
			buff[a].hp += _b.hp;
			atk += _b.atk;
			hp += _b.hp;
			hpMax += _b.hp;
			return;
		}
	}
	buff.push_back(_b);
	atk += _b.atk;
	hp += _b.hp;
	hpMax += _b.hp;
}

//Add effect to list
void Unit::addEffect(eEffect e) {
	Effect _e = game->cl.el.find(e);
	for (int a = 0; a < effect.size(); ++a) {
		if (effect[a].effect == e) {
			effect[a].count += _e.count;
			return;
		}
	}
	effect.push_back(_e);
}

//Remove effect from list
void Unit::removeEffect(eEffect e) {
	Effect _e = game->cl.el.find(e);
	for (int a = 0; a < effect.size(); ++a) {
		if (effect[a].effect == e) {
			effect[a].count -= _e.count;
			if (effect[a].count == 0) { effect.erase(effect.begin() + a); }
			return;
		}
	}
}

//Check if unit has died
void Unit::update(bool& r) {
	if (hp < 1) {
		for (int a = 0; a < game->unit.size(); ++a) { game->unit[a]->onDeath(*this); }
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
	header[1].createFromString(s);
	for (int a = 0; a < header[1].size; ++a) {
		if (s[a] != ':' && s[a] != ' ') { header[1].buffer[a].Attributes = col; }
		if (s[a] == ' ') {
			if (col == COLOR_LTBLUE) { col = COLOR_GREEN; }
			else { col = COLOR_RED; }
		}
	}
}

//Generate sidebar details
void Unit::generateDetails() {
	std::string s;
	s.resize(name.size());
	std::transform(name.begin(), name.end(), s.begin(), ::toupper);
	switch (tribe) {
	case TRIBE_NONE:
		s += " - MINION";
		break;
	case TRIBE_GENERAL:
		s += " - GENERAL";
		break;
	case TRIBE_ARCANYST:
		s += " - ARCANYST";
		break;
	case TRIBE_PET:
		s += " - PET";
		break;
	case TRIBE_GOLEM:
		s += " - GOLEM";
		break;
	case TRIBE_MECH:
		s += " - MECH";
		break;
	case TRIBE_DERVISH:
		s += " - DERVISH";
		break;
	case TRIBE_VESPYR:
		s += " - VESPYR";
		break;
	case TRIBE_STRUCTURE:
		s += " - STRUCTURE";
		break;
	}
	header[0].createFromString(s);
	for (int a = name.size(); a < header[0].size; ++a) { header[0].buffer[a].Attributes = COLOR_GRAY; }
	updateDetailStats();
}

//When a unit is summoned
void Unit::onSummon(Unit& u) {

	//When this is summoned (Opening Gambit)
	if (&u == this) {
		moved = true;
		attacked = true;
		switch (skill.skill) {
		case SKILL_AETHERMASTER:
			++player->replaces;
			player->general->addEffect(EFFECT_AETHERMASTER);
			break;
		case SKILL_AZURE_HERALD:
			player->general->hp = min(player->general->hp + 3, player->general->hpMax);
			break;
		case SKILL_GHOST_LYNX:
			game->highlightSelectable(TARGET_MINION_NEAR_UNIT, this);
			if (game->selectable.size() > 0) { game->callback = Callback(this, SKILL_GHOST_LYNX); }
			break;
		}
	}

	//When something else is summoned
	else {
		switch (skill.skill) {
		case SKILL_ARAKI_HEADHUNTER:
			if (u.player == player) {
				switch (u.skill.skill) {
				case SKILL_AZURE_HERALD:
				case SKILL_GHOST_LYNX:
					addBuff(BUFF_ARAKI_HEADHUNTER);
					break;
				}
			}
			break;
		}
	}

}

//When a unit dies
void Unit::onDeath(Unit& u) {

	//When this unit dies
	if (&u == this) {

		//Remove tile reference
		tile->unit = nullptr;
		for (int a = 0; a < game->hostile.size(); ++a) {
			if (game->hostile[a] == tile) {
				game->hostile.erase(game->hostile.begin() + a);
				break;
			}
		}

		//Move to graveyard
		for (int a = 0; a < game->unit.size(); ++a) {
			if (game->unit[a] == this) {
				game->unit.erase(game->unit.begin() + a);
				game->grave.push_back(this);
				break;
			}
		}

		//Remove effects
		switch (skill.skill) {
		case SKILL_AETHERMASTER:
			player->replaces = max(player->replaces - 1, 0);
			player->general->removeEffect(EFFECT_AETHERMASTER);
			break;
		}

	}

}

//When a unit attacks
void Unit::onAttack(Unit& u1, Unit& u2) {}

//When a player's turn ends
void Unit::onTurnEnd(Player& p) {

	//When this unit's player ends turn
	if (&p == player) {
		for (int a = 0; a < effect.size(); ++a) {
			switch (effect[a].effect) {
			case EFFECT_AETHERMASTER:
				player->replaces += effect[a].count;
				break;
			}
		}
	}

}

//When a player's turn starts
void Unit::onTurnStart(Player& p) {}

//Effect callback
void Unit::callback(BoardTile* t) {
	switch (game->callback.skill) {
	case SKILL_GHOST_LYNX:
		std::vector<BoardTile*> v;
		for (int a = 0; a < 9; ++a) {
			for (int b = 0; b < 5; ++b) {
				if (!game->map.tile[a][b].unit) {
					v.push_back(&game->map.tile[a][b]);
				}
			}
		}
		int i = rand() % v.size();
		t->unit->setPos(v[i]->pos.x, v[i]->pos.y);
		break;
	}
	game->callback = Callback();
}
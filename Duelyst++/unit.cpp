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
	dead = false;
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
			if (!_b.stacking) { return; }
			buff[a].cost += _b.cost;
			buff[a].atk += _b.atk;
			buff[a].hp += _b.hp;
			updateStatBuffs();
			return;
		}
	}
	buff.push_back(_b);
	updateStatBuffs();
}

//Remove buff from list
void Unit::removeBuff(eBuff b) {
	Buff _b = game->cl.el.find(b);
	for (int a = 0; a < buff.size(); ++a) {
		if (buff[a].buff == b) {
			buff.erase(buff.begin() + a);
			break;
		}
	}
	updateStatBuffs();
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
			break;
		}
	}
}

//Check if unit has died
void Unit::update(bool& r) {
	if (hp < 1) {
		game->sendOnDeath(this);
		dead = true;
		r = true;
	}
}

//Update attack/health/cost when buffs change
void Unit::updateStatBuffs() {
	int _cost = 0;
	int _atk = 0;
	int _hp = 0;
	int hpDelta = hp - hpMax;
	for (int a = 0; a < buff.size(); ++a) {
		_cost += buff[a].cost;
		_atk += buff[a].atk;
		_hp += buff[a].hp;
	}
	Unit* o = dynamic_cast<Unit*>(original);
	cost = max(o->cost + _cost, 0);
	atk = max(o->atk + _atk, 0);
	hpMax = o->hpMax + _hp;
	hp = hpMax + hpDelta;
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
		std::string s = "";
		if (buff[a].hp == 0 && buff[a].atk != 0) { s += (buff[a].atk > 0 ? "+" : "-") + std::to_string(abs(buff[a].atk)) + " Attack"; }
		else if (buff[a].atk == 0 && buff[a].hp != 0) { s += (buff[a].hp > 0 ? "+" : "-") + std::to_string(abs(buff[a].hp)) + " Health"; }
		else if (buff[a].atk != 0 && buff[a].hp != 0) { s += (buff[a].atk > 0 ? "+" : "-") + std::to_string(buff[a].atk) + (buff[a].hp > 0 ? "/+" : "/-") + std::to_string(abs(buff[a].hp)); }
		else if (buff[a].cost == 0) { s += "+0/+0"; }
		if (buff[a].cost != 0) {
			if (s != "") { s += ", "; }
			s += (buff[a].cost > 0 ? "+" : "-") + std::to_string(abs(buff[a].cost)) + " Cost";
		}
		Sprite _s = Sprite();
		_s.createFromString(s);
		_s.setCol(COLOR_GRAY);
		rm.render(_s, 72, y); y += 2;
	}
	if (token != nullptr) {
		if (y < 7) { y = 7; }
		rm.render(divider, 66, y);
		y += 2;
		token->drawDetails(rm, y);
	}
}

//Can unit attack target
bool Unit::canAttack(Unit* u) {
	if (u != nullptr) {
		if (atk > 0) {
			if (abs(tile->pos.x - u->tile->pos.x) < 2 && abs(tile->pos.y - u->tile->pos.y) < 2) {
				return true;
			}
		}
	}
	return false;
}

//Can unit move
bool Unit::isMoveable() {
	if (tribe == TRIBE_STRUCTURE) { return false; }
	return true;
}

//Is unit flying
bool Unit::isFlying() {
	switch (skill.skill) {
	case SKILL_FLYING:
		return true;
	}
	return false;
}

//Attack enemy
void Unit::attack(Unit* u, bool counter) {
	u->hp -= atk;
	switch (skill.skill) {
	case SKILL_BLUETIP_SCORPION:
		if (u->tribe != TRIBE_GENERAL) { u->hp -= atk; }
		break;
	}
	if (!counter) {
		moved = true;
		attacked = true;
		if (u->canAttack(this)) { u->attack(this, true); }
	}
	game->sendOnDamage(this, u);
}

//When a unit is summoned
void Unit::onSummon(Unit* u, bool actionBar) {

	//If on board
	if (tile != nullptr) {

		//When this is summoned
		if (u == this) {

			//Exhaust
			moved = true;
			attacked = true;

			//Skills (Opening Gambit)
			switch (skill.skill) {
			case SKILL_ABJUDICATOR:
				for (int a = 0; a < player->hand.size(); ++a) {
					if (player->hand[a]->type == CARD_SPELL) {
						dynamic_cast<Spell*>(player->hand[a])->addBuff(BUFF_ABJUDICATOR);
					}
				}
				break;
			case SKILL_AETHERMASTER:
				++player->replaces;
				player->general->addEffect(EFFECT_AETHERMASTER);
				break;
			case SKILL_ALCUIN_LOREMASTER:
				if (player->hand.size() < 6) {
					for (int a = game->grave.size() - 1; a > -1; --a) {
						if (game->grave[a]->type == CARD_SPELL) {
							Spell* s = new Spell(*(dynamic_cast<Spell*>(game->grave[a]->original)));
							game->setContext(s, player);
							player->hand.push_back(s);
							break;
						}
					}
				}
				break;
			case SKILL_AZURE_HERALD:
				player->general->hp = min(player->general->hp + 3, player->general->hpMax);
				break;
			case SKILL_BLAZE_HOUND:
				game->player[0].draw();
				game->player[1].draw();
				break;
			case SKILL_BLISTERING_SKORN:
				for (int a = 0; a < game->unit.size(); ++a) {
					--game->unit[a]->hp;
					game->sendOnDamage(this, game->unit[a]);
				}
				break;
			case SKILL_BLOODTEAR_ALCHEMIST:
				game->highlightSelectable(TARGET_ENEMY);
				if (game->selectable.size() > 0) { game->callback = Callback(this, nullptr, nullptr, SKILL_BLOODTEAR_ALCHEMIST); }
				break;
			case SKILL_GHOST_LYNX:
				game->highlightSelectable(TARGET_MINION_NEAR_UNIT, this);
				if (game->selectable.size() > 0) { game->callback = Callback(this, nullptr, nullptr, SKILL_GHOST_LYNX); }
				break;
			}

			//Buffs
			for (int a = 0; a < buff.size(); ++a) {
				switch (buff[a].buff) {
				case BUFF_DARKFIRE_SACRIFICE:
					removeBuff(BUFF_DARKFIRE_SACRIFICE);
					for (int b = 0; b < player->hand.size(); ++b) {
						if (player->hand[b]->type == CARD_UNIT) {
							dynamic_cast<Unit*>(player->hand[b])->removeBuff(BUFF_DARKFIRE_SACRIFICE);
						}
					}
					for (int b = 0; b < player->deck.size(); ++b) {
						if (player->deck[b]->type == CARD_UNIT) {
							dynamic_cast<Unit*>(player->deck[b])->removeBuff(BUFF_DARKFIRE_SACRIFICE);
						}
					}
					break;
				}
			}

		}

		//When something else is summoned
		else {
			switch (skill.skill) {
			case SKILL_ARAKI_HEADHUNTER:
				if (u->player == player) {
					switch (u->skill.skill) {
					case SKILL_ABJUDICATOR:
					case SKILL_ALCUIN_LOREMASTER:
					case SKILL_AZURE_HERALD:
					case SKILL_BLAZE_HOUND:
					case SKILL_BLISTERING_SKORN:
					case SKILL_BLOODTEAR_ALCHEMIST:
					case SKILL_GHOST_LYNX:
						if (actionBar) { addBuff(BUFF_ARAKI_HEADHUNTER); }
						break;
					}
				}
				break;
			}
		}

	}

}

//When a unit dies
void Unit::onDeath(Unit* u) {

	//If on board
	if (tile != nullptr) {

		//When this unit dies (Dying Wish)
		if (u == this) {
			switch (skill.skill) {
			case SKILL_AETHERMASTER:
				player->replaces = max(player->replaces - 1, 0);
				player->general->removeEffect(EFFECT_AETHERMASTER);
				break;
			case SKILL_AZURE_HORN_SHAMAN:
				for (int a = max(tile->pos.x - 1, 0); a < min(tile->pos.x + 2, 9); ++a) {
					for (int b = max(tile->pos.y - 1, 0); b < min(tile->pos.y + 2, 5); ++b) {
						if (game->map.tile[a][b].unit != nullptr && game->map.tile[a][b].unit->player == player) {
							if (game->map.tile[a][b].unit != player->general) {
								game->map.tile[a][b].unit->addBuff(BUFF_AZURE_HORN_SHAMAN);
							}
						}
					}
				}
				break;
			}
		}

	}

}

//When a unit attacks
void Unit::onAttack(Unit* u1, Unit* u2) {}

//When a unit is damaged
void Unit::onDamage(Unit* u1, Unit* u2) {

	//If on board
	if (tile != nullptr) {
		switch (skill.skill) {
		case SKILL_CHAOS_ELEMENTAL:
			if (u2 == this) {
				BoardTile* t = game->map.getRandom();
				if (t != nullptr) { setPos(t->pos.x, t->pos.y); }
			}
			break;
		}
	}

	//If in hand/deck
	else {
		switch (skill.skill) {
		case SKILL_CHAKKRAM:
			if (u2 == player->general) {
				if (&game->player[game->turn] != player) {
					addBuff(BUFF_CHAKKRAM);
				}
			}
			break;
		}
	}

}

//When this card is replaced
bool Unit::onReplace() {
	switch (skill.skill) {
	case SKILL_ASTRAL_CRUSADER:
		addBuff(BUFF_ASTRAL_CRUSADER);
		return true;
	}
	return true;
}

//When a player's turn ends
void Unit::onTurnEnd(Player* p) {

	//If on board
	if (tile != nullptr) {

		//When this unit's player ends turn
		if (p == player) {
			switch (skill.skill) {
			case SKILL_BASTION:
				for (int a = 0; a < game->unit.size(); ++a) {
					if (game->unit[a]->player == player && game->unit[a] != this) {
						if (game->unit[a] != player->general) {
							game->unit[a]->addBuff(BUFF_BASTION);
						}
					}
				}
				break;
			}
			for (int a = 0; a < effect.size(); ++a) {
				switch (effect[a].effect) {
				case EFFECT_AETHERMASTER:
					player->replaces += effect[a].count;
					break;
				}
			}
		}

	}

	//Anywhere
	switch (skill.skill) {
	case SKILL_CHAKKRAM:
		if (p == player) {
		removeBuff(BUFF_CHAKKRAM);
		}
		break;
	}

}

//When a player's turn starts
void Unit::onTurnStart(Player* p) {}

//Effect callback
void Unit::callback(BoardTile* t) {
	switch (game->callback.skill) {
	case SKILL_BLOODTEAR_ALCHEMIST:
		if (t->unit != nullptr) {
			--t->unit->hp;
			game->sendOnDamage(this, t->unit);
		}
		break;
	case SKILL_GHOST_LYNX:
		BoardTile* t2 = game->map.getRandom();
		if (t2 != nullptr) { t->unit->setPos(t2->pos.x, t2->pos.y); }
		break;
	}
	game->callback = Callback();
}
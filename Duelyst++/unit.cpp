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
	celerityMoved = true;
	celerityAttacked = true;
	if (path == "") { sprite.resize(5, 5); }
	else if (tribe == TRIBE_GENERAL) { sprite.createFromFile("resources/generals/" + path + ".txt"); }
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
void Unit::removeBuff(eBuff b, bool allStacks) {
	Buff _b = game->cl.el.find(b);
	for (int a = 0; a < buff.size(); ++a) {
		if (buff[a].buff == b) {
			if (allStacks) { buff.erase(buff.begin() + a); }
			else {
				buff[a].cost -= _b.cost;
				buff[a].atk -= _b.atk;
				buff[a].hp -= _b.hp;
				if (buff[a].cost == 0 && buff[a].atk == 0 && buff[a].hp == 0) { buff.erase(buff.begin() + a); }
			}
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
void Unit::removeEffect(eEffect e, bool allStacks) {
	Effect _e = game->cl.el.find(e);
	for (int a = 0; a < effect.size(); ++a) {
		if (effect[a].effect == e) {
			if (allStacks) { effect.erase(effect.begin() + a); }
			else {
				effect[a].count -= _e.count;
				if (effect[a].count == 0) { effect.erase(effect.begin() + a); }
			}
			break;
		}
	}
}

//Check if unit has died
void Unit::update(bool& r) {
	if (hp < 1) {
		game->em.sendOnDeath(this);
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
	case TRIBE_WARMASTER:
		s += " - WARMASTER";
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
		case EFFECT_DARKFIRE_SACRIFICE:
			effect[a].sprite[1].buffer[23].Char.AsciiChar = std::to_string(effect[a].count * 2)[0];
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
			if (isRanged()) { return true; }
			else if (abs(tile->pos.x - u->tile->pos.x) < 2 && abs(tile->pos.y - u->tile->pos.y) < 2) { return true; }
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
	case SKILL_BLACK_LOCUST:
		return true;
	}
	return false;
}

//Is unit ranged
bool Unit::isRanged() {
	switch (skill.skill) {
	case SKILL_ARROW_WHISTLER:
	case SKILL_CAPTAIN_HANK_HART:
		return true;
	}
	return false;
}

//Does unit have provoke
bool Unit::isProvoking() {
	switch (skill.skill) {
	case SKILL_BLOOD_TAURA:
	case SKILL_BONEREAPER:
		return true;
	}
	return false;
}

//Is unit being provoked
bool Unit::isProvoked() {
	for (int a = max(tile->pos.x - 1, 0); a < min(tile->pos.x + 2, 9); ++a) {
		for (int b = max(tile->pos.y - 1, 0); b < min(tile->pos.y + 2, 5); ++b) {
			if (game->map.tile[a][b].unit != nullptr) {
				if (game->map.tile[a][b].unit->player != player) {
					if (game->map.tile[a][b].unit->isProvoking()) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

//Does unit have celerity
bool Unit::hasCelerity() {
	switch (skill.skill) {
	case SKILL_CELERITY:
		return true;
	}
	return false;
}

//Attack enemy
void Unit::attack(Unit* u, bool counter) {
	int damage = atk;
	switch (skill.skill) {
	case SKILL_BLUETIP_SCORPION:
		if (u->tribe != TRIBE_GENERAL) { damage += atk; }
		break;
	}
	u->dealDamage(this, damage);
	game->em.sendOnAttack(this, u, counter);
	if (!counter) {
		if (!celerityAttacked && !moved) {
			celerityMoved = true;
			celerityAttacked = true;
		}
		else {
			moved = true;
			attacked = true;
			celerityMoved = true;
			celerityAttacked = true;
		}
		if (u->canAttack(this)) { u->attack(this, true); }
	}
}

//Deal damage to this
int Unit::dealDamage(Unit* u, int damage) {
	if (damage < 0) {
		if (hp - damage > hpMax) {
			damage = -(hpMax - hp);
			hp = hpMax;
		}
		else { hp -= damage; }
		game->em.sendOnHeal(u, this, -damage);
	}
	else {
		hp -= damage;
		game->em.sendOnDamage(u, this, damage);
	}
	return damage;
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
			celerityMoved = true;
			celerityAttacked = true;

			//From action bar (Opening Gambit)
			if (actionBar) {
				switch (skill.skill) {
				case SKILL_ABJUDICATOR:
					for (int a = 0; a < player->hand.size(); ++a) {
						if (player->hand[a]->type == CARD_SPELL) {
							dynamic_cast<Spell*>(player->hand[a])->addBuff(BUFF_ABJUDICATOR);
						}
					}
					break;
				case SKILL_ALCUIN_LOREMASTER:
					for (int a = game->grave.size() - 1; a > -1; --a) {
						if (game->grave[a]->type == CARD_SPELL) {
							player->addToHand(game->grave[a]->original, true);
							break;
						}
					}
					break;
				case SKILL_ASH_MEPHYT:
					for (int a = 0; a < 2; ++a) {
						BoardTile* t = game->map.getRandomNear(tile->pos.x, tile->pos.y);
						if (t != nullptr) {
							Unit* u2 = new Unit(*(dynamic_cast<Unit*>(original)));
							game->setContext(u2, player);
							game->summon(u2, t->pos.x, t->pos.y, false);
						}
					}
					break;
				case SKILL_AZURE_HERALD:
					player->general->dealDamage(this, -3);
					break;
				case SKILL_BLAZE_HOUND:
					game->player[0].draw();
					game->player[1].draw();
					break;
				case SKILL_BLISTERING_SKORN:
					for (int a = 0; a < game->unit.size(); ++a) { game->unit[a]->dealDamage(this, 1); }
					break;
				case SKILL_BLOODTEAR_ALCHEMIST:
					game->highlightSelectable(TARGET_ENEMY);
					if (game->selectable.size() > 0) { game->callback = Callback(this, nullptr, nullptr, SKILL_BLOODTEAR_ALCHEMIST); }
					break;
				case SKILL_CROSSBONES:
					game->highlightSelectable(TARGET_ENEMY_RANGED);
					if (game->selectable.size() > 0) { game->callback = Callback(this, nullptr, nullptr, SKILL_CROSSBONES); }
					break;
				case SKILL_DANCING_BLADES:
					if (true) { //Why is this necessary to initialize a local variable
						int _x = tile->pos.x;
						&game->player[0] == player ? ++_x : --_x;
						if (_x > -1 && _x < 9) {
							if (game->map.tile[_x][tile->pos.y].unit != nullptr) {
								if (game->map.tile[_x][tile->pos.y].unit->tribe != TRIBE_GENERAL) {
									game->map.tile[_x][tile->pos.y].unit->dealDamage(this, 3);
								}
							}
						}
					}
					break;
				case SKILL_DEATHBLIGHTER:
					for (int a = max(tile->pos.x - 1, 0); a < min(tile->pos.x + 2, 9); ++a) {
						for (int b = max(tile->pos.y - 1, 0); b < min(tile->pos.y + 2, 5); ++b) {
							if (game->map.tile[a][b].unit != nullptr) {
								if (game->map.tile[a][b].unit->player != player) {
									if (game->map.tile[a][b].unit->tribe != TRIBE_GENERAL) {
										game->map.tile[a][b].unit->dealDamage(this, 3);
									}
								}
							}
						}
					}
					break;
				case SKILL_GHOST_LYNX:
					game->highlightSelectable(TARGET_MINION_NEAR_UNIT, this);
					if (game->selectable.size() > 0) { game->callback = Callback(this, nullptr, nullptr, SKILL_GHOST_LYNX); }
					break;
				}
			}

			//Summoned from anywhere (apply global effects)
			else {
				switch (skill.skill) {
				case SKILL_AETHERMASTER:
					++player->replaces;
					break;
				case SKILL_ARCHON_SPELLBINDER:
					for (int a = 0; a < player->enemy->hand.size(); ++a) {
						if (player->enemy->hand[a]->type == CARD_SPELL) {
							dynamic_cast<Spell*>(player->enemy->hand[a])->addBuff(BUFF_ARCHON_SPELLBINDER);
						}
					}
					for (int a = 0; a < player->enemy->deck.size(); ++a) {
						if (player->enemy->deck[a]->type == CARD_SPELL) {
							dynamic_cast<Spell*>(player->enemy->deck[a])->addBuff(BUFF_ARCHON_SPELLBINDER);
						}
					}
					break;
				case SKILL_ARROW_WHISTLER:
					for (int a = 0; a < game->unit.size(); ++a) {
						if (game->unit[a]->player == player) {
							if (game->unit[a] != this) {
								if (game->unit[a]->isRanged()) {
									game->unit[a]->addBuff(BUFF_ARROW_WHISTLER);
								}
							}
						}
					}
					break;
				}
			}

		}

		//When something else is summoned
		else {

			//Skills
			switch (skill.skill) {
			case SKILL_ARAKI_HEADHUNTER:
				if (u->player == player) {
					if (actionBar) {
						switch (u->skill.skill) {
						case SKILL_ABJUDICATOR:
						case SKILL_ALCUIN_LOREMASTER:
						case SKILL_ASH_MEPHYT:
						case SKILL_AZURE_HERALD:
						case SKILL_BLAZE_HOUND:
						case SKILL_BLISTERING_SKORN:
						case SKILL_BLOODTEAR_ALCHEMIST:
						case SKILL_CROSSBONES:
						case SKILL_DANCING_BLADES:
						case SKILL_DEATHBLIGHTER:
						case SKILL_GHOST_LYNX:
							if (actionBar) { addBuff(BUFF_ARAKI_HEADHUNTER); }
							break;
						}
					}
				}
				break;
			case SKILL_ARROW_WHISTLER:
				if (u->player == player) {
					if (u->isRanged()) {
						u->addBuff(BUFF_ARROW_WHISTLER);
					}
				}
				break;
			case SKILL_CRIMSON_OCULUS:
				if (u->player != player) { addBuff(BUFF_CRIMSON_OCULUS); }
				break;
			}

			//Effects
			for (int a = 0; a < effect.size(); ++a) {
				switch (effect[a].effect) {
				case EFFECT_DARKFIRE_SACRIFICE:
					removeEffect(EFFECT_DARKFIRE_SACRIFICE, true);
					u->removeBuff(BUFF_DARKFIRE_SACRIFICE, true);
					for (int b = 0; b < player->hand.size(); ++b) {
						if (player->hand[b]->type == CARD_UNIT) {
							dynamic_cast<Unit*>(player->hand[b])->removeBuff(BUFF_DARKFIRE_SACRIFICE, true);
						}
					}
					for (int b = 0; b < player->deck.size(); ++b) {
						if (player->deck[b]->type == CARD_UNIT) {
							dynamic_cast<Unit*>(player->deck[b])->removeBuff(BUFF_DARKFIRE_SACRIFICE, true);
						}
					}
					break;
				}

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
				break;
			case SKILL_ARCHON_SPELLBINDER:
				for (int a = 0; a < player->enemy->hand.size(); ++a) {
					if (player->enemy->hand[a]->type == CARD_SPELL) {
						dynamic_cast<Spell*>(player->enemy->hand[a])->removeBuff(BUFF_ARCHON_SPELLBINDER, false);
					}
				}
				for (int a = 0; a < player->enemy->deck.size(); ++a) {
					if (player->enemy->deck[a]->type == CARD_SPELL) {
						dynamic_cast<Spell*>(player->enemy->deck[a])->removeBuff(BUFF_ARCHON_SPELLBINDER, false);
					}
				}
				break;
			case SKILL_ARROW_WHISTLER:
				for (int a = 0; a < game->unit.size(); ++a) {
					if (game->unit[a]->player == player) {
						if (game->unit[a] != this) {
							if (game->unit[a]->isRanged()) {
								game->unit[a]->removeBuff(BUFF_ARROW_WHISTLER, false);
							}
						}
					}
				}
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
void Unit::onAttack(Unit* u1, Unit* u2, bool counter) {

	//If on board
	if (tile != nullptr) {

		//If ally attacked
		if (u1->player == player) {

			//Skills
			switch (skill.skill) {
			case SKILL_DAY_WATCHER:
				if (!counter) {
					if (u1->tribe != TRIBE_GENERAL) {
						player->general->dealDamage(this, -1);
					}
				}
				break;
			}

		}

	}

}

//When a unit is damaged
void Unit::onDamage(Unit* u1, Unit* u2, int damage) {

	//If on board
	if (tile != nullptr) {

		//If this dealt damage
		if (u1 == this) {
			switch (skill.skill) {
			case SKILL_CAPTAIN_HANK_HART:
				if (hp > 0) { hp = min(hp + damage, hpMax); }
				break;
			}
		}

		//If this was damaged
		if (u2 == this) {
			switch (skill.skill) {
			case SKILL_CHAOS_ELEMENTAL:
				BoardTile* t = game->map.getRandom();
				if (t != nullptr) { setPos(t->pos.x, t->pos.y); }
				game->em.sendOnMove(this, true);
				break;
			}
		}

	}

	//If in hand/deck
	else {
		switch (skill.skill) {
		case SKILL_BLOOD_TAURA:
			if (u2 == player->general) {
				removeBuff(BUFF_BLOOD_TAURA, true);
				for (int a = 0; a < 25 - player->general->hp; ++a) { addBuff(BUFF_BLOOD_TAURA); }
			}
			break;
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

//When a unit is healed
void Unit::onHeal(Unit* u1, Unit* u2, int heal) {

	//If in hand/deck
	if (tile == nullptr) {
		switch (skill.skill) {
		case SKILL_BLOOD_TAURA:
			if (u2 == player->general) {
				removeBuff(BUFF_BLOOD_TAURA, true);
				for (int a = 0; a < 25 - player->general->hp; ++a) { addBuff(BUFF_BLOOD_TAURA); }
			}
			break;
		}
	}

}

//When a minion moves or is moved
void Unit::onMove(Unit* u, bool byEffect) {

	//If on board
	if (tile != nullptr) {

		//If this moved
		if (u == this) {
			switch (skill.skill) {
			case SKILL_BLACK_LOCUST:
				if (!byEffect) {
					BoardTile* t = game->map.getRandomNear(tile->pos.x, tile->pos.y);
					if (t != nullptr) {
						Unit* u2 = new Unit(*(dynamic_cast<Unit*>(original)));
						game->setContext(u2, player);
						game->summon(u2, t->pos.x, t->pos.y, false);
					}
				}
				break;
			}
		}

	}

}

//When a card is drawn
void Unit::onDraw(Card* c, bool fromDeck) {

	//If on board
	if (tile != nullptr) {

		//Skills
		switch (skill.skill) {
		case SKILL_ARCHON_SPELLBINDER:
			if (!fromDeck) {
				if (c->player != player && c->type == CARD_SPELL) {
					dynamic_cast<Spell*>(c)->addBuff(BUFF_ARCHON_SPELLBINDER);
				}
			}
			break;
		}

		//Effects
		for (int a = 0; a < effect.size(); ++a) {
			switch (effect[a].effect) {
			case EFFECT_DARKFIRE_SACRIFICE:
				if (!fromDeck) {
					if (c->player == player && c->type == CARD_UNIT) {
						for (int b = 0; b < effect[a].count; ++b) {
							dynamic_cast<Unit*>(c)->addBuff(BUFF_DARKFIRE_SACRIFICE);
						}
					}
				}
				break;
			}
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

	//Refresh
	moved = false;
	attacked = false;
	if (hasCelerity()) {
		celerityMoved = false;
		celerityAttacked = false;
	}

	//If on board
	if (tile != nullptr) {

		//When this unit's player ends turn
		if (p == player) {

			//Skills
			switch (skill.skill) {
			case SKILL_AETHERMASTER:
				++player->replaces;
				break;
			case SKILL_BASTION:
				for (int a = 0; a < game->unit.size(); ++a) {
					if (game->unit[a]->player == player && game->unit[a] != this) {
						if (game->unit[a] != player->general) {
							game->unit[a]->addBuff(BUFF_BASTION);
						}
					}
				}
				break;
			case SKILL_BONEREAPER:
				for (int a = max(tile->pos.x - 1, 0); a < min(tile->pos.x + 2, 9); ++a) {
					for (int b = max(tile->pos.y - 1, 0); b < min(tile->pos.y + 2, 5); ++b) {
						if (game->map.tile[a][b].unit != nullptr) {
							if (game->map.tile[a][b].unit->player != player) {
								if (game->map.tile[a][b].unit->tribe != TRIBE_GENERAL) {
									game->map.tile[a][b].unit->dealDamage(this, 2);
								}
							}
						}
					}
				}
				break;
			}

		}

	}

	//Anywhere
	switch (skill.skill) {
	case SKILL_CHAKKRAM:
		if (p == player) { removeBuff(BUFF_CHAKKRAM, true); }
		break;
	}

}

//When a player's turn starts
void Unit::onTurnStart(Player* p) {

	//If on board
	if (tile != nullptr) {

		//If this unit's player started turn
		if (p == player) {

			//Skills
			switch (skill.skill) {
			case SKILL_DARK_NEMESIS:
				player->enemy->general->dealDamage(this, 4);
				addBuff(BUFF_DARK_NEMESIS);
				break;
			}

		}

	}

}

//Effect callback
void Unit::callback(BoardTile* t) {
	switch (game->callback.skill) {
	case SKILL_BLOODTEAR_ALCHEMIST:
		if (t->unit != nullptr) { t->unit->dealDamage(this, 1); }
		break;
	case SKILL_CROSSBONES:
		if (t->unit != nullptr) { t->unit->dead = true; }
		break;
	case SKILL_GHOST_LYNX:
		BoardTile* t2 = game->map.getRandom();
		if (t2 != nullptr) { t->unit->setPos(t2->pos.x, t2->pos.y); }
		game->em.sendOnMove(t2->unit, true);
		break;
	}
	game->callback = Callback();
}
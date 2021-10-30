//Include
#include "game.h"

//Game constructor / destructor
Spell::Spell(eFaction _faction,  eTarget _target, int _cost, std::string path, std::string _name) {
	type = CARD_SPELL;
	faction = _faction;
	target = _target;
	cost = _cost;
	name = _name;
	if (path == "") { sprite.resize(5, 5); }
	else { sprite.createFromFile("resources/spells/" + path + ".txt"); }
	generateDetails();
	game = nullptr;
	player = nullptr;
}
Spell::~Spell() {}

//Generate sidebar details
void Spell::generateDetails() {
	std::string s;
	s.resize(name.size());
	std::transform(name.begin(), name.end(), s.begin(), ::toupper);
	s += " - SPELL";
	header[0].createFromString(s);
	for (int a = name.size(); a < header[0].size; ++a) { header[0].buffer[a].Attributes = COLOR_GRAY; }
	updateDetailStats();
}

//Update card stats
void Spell::updateDetailStats() {
	std::string s = "COST:" + std::to_string(cost);
	header[1].createFromString(s);
	for (int a = 0; a < header[1].size; ++a) { if (s[a] != ':') { header[1].buffer[a].Attributes = COLOR_LTBLUE; } }
}

//Draw card details
void Spell::drawDetails(Renderer& rm, int& y) {
	sprite.setCol(COLOR_LTWHITE);
	rm.render(sprite, 66, y);
	rm.render(header[0], 72, y); ++y;
	updateDetailStats();
	rm.render(header[1], 72, y); y+= 2;
	for (int a = 0; a < spell.sprite.size(); ++a) {
		rm.render(spell.sprite[a], 72, y);
		if (a == spell.sprite.size() - 1) { ++y; }
		++y;
	}
}

//On card use
void Spell::onUse(BoardTile* t) {
	switch (spell.spell) {
	case SPELL_BREATH_OF_THE_UNBORN:
		for (int a = 0; a < game->unit.size(); ++a) {
			if (game->unit[a]->tribe != TRIBE_GENERAL) {
				if (game->unit[a]->player == player) { game->unit[a]->hp = game->unit[a]->hpMax; }
				else {
					game->unit[a]->hp -= 2;
					game->sendOnDamage(nullptr, game->unit[a]);
				}
			}
		}
		break;
	case SPELL_CONSUMING_REBIRTH:
		if (t->unit != nullptr) {
			token = t->unit->original;
			t->unit->hp = -999;
			game->lateCallback.push_back(Callback(nullptr, this, t->unit->tile, SKILL_NONE));
		}
		break;
	case SPELL_DAEMONIC_LURE:
		game->highlightSelectable(TARGET_TILE);
		if (game->selectable.size() > 0) {
			game->callback = Callback(nullptr, this, nullptr, SKILL_NONE);
			token = t->unit;
		}
		else {
			--t->unit->hp;
			game->sendOnDamage(nullptr, t->unit);
		}
		break;
	case SPELL_DARK_SEED:
		if (t->unit != nullptr) {
			t->unit->hp -= player == &game->player[0] ? game->player[1].hand.size() : game->player[0].hand.size();
			game->sendOnDamage(nullptr, t->unit);
		}
		break;
	case SPELL_DARK_TRANSFORMATION:
		if (true) { //Need this to initialize variable
			Unit* u1 = t->unit;
			if (u1 != nullptr) {
				t->unit->dead = true;
				Unit* u2 = new Unit(*(dynamic_cast<Unit*>(token)));
				game->setContext(u2, player);
				game->summon(u2, t->pos.x, t->pos.y);
				game->sendOnDamage(nullptr, u1);
				game->sendOnDeath(u1);
			}
		}
		break;
	case SPELL_DARKFIRE_SACRIFICE:
		if (t->unit != nullptr) {
			t->unit->hp = -999;
			for (int a = 0; a < player->hand.size(); ++a) {
				if (player->hand[a]->type == CARD_UNIT) {
					dynamic_cast<Unit*>(player->hand[a])->addBuff(BUFF_DARKFIRE_SACRIFICE);
				}
			}
			for (int a = 0; a < player->deck.size(); ++a) {
				if (player->deck[a]->type == CARD_UNIT) {
					dynamic_cast<Unit*>(player->deck[a])->addBuff(BUFF_DARKFIRE_SACRIFICE);
				}
			}
		}
		break;
	}
}

//Immediate callback
void Spell::callback(BoardTile* t) {
	switch (spell.spell) {
	case SPELL_DAEMONIC_LURE:
		Unit* u = dynamic_cast<Unit*>(token);
		u->setPos(t->pos.x, t->pos.y);
		--u->hp;
		game->sendOnDamage(nullptr, u);
		break;
	}
	game->callback = Callback();
}

//Late callback
void Spell::lateCallback() {
	switch (spell.spell) {
	case SPELL_CONSUMING_REBIRTH:
		if (game->lateCallback[0].tile->unit == nullptr) {
			Unit* t = new Unit(*(dynamic_cast<Unit*>(token)));
			game->setContext(t, player);
			game->summon(t, game->lateCallback[0].tile->pos.x, game->lateCallback[0].tile->pos.y);
			game->unit.back()->addBuff(BUFF_CONSUMING_REBIRTH);
		}
		break;
	}
}
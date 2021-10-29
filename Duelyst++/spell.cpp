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
	}
}

//Immediate callback
void Spell::callback(BoardTile* t) {}

//Late callback
void Spell::lateCallback() {}
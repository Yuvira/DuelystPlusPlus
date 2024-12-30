//Include
#include "game.h"

#pragma region Constructors / Initialization

//Game constructors
Spell::Spell() : Spell(FACTION_NEUTRAL, TARGET_ANY, 0, "", "???") {}
Spell::Spell(eFaction _faction, eTarget _target, int _cost, std::string path, std::string _name) {
	cardType = CARD_SPELL;
	faction = _faction;
	targetMode = _target;
	cost = _cost;
	name = _name;
	if (path == "")
		sprite.Resize(5, 5);
	else
		sprite.CreateFromFile("resources/spells/" + path + ".txt");
	GenerateDetails();
	game = nullptr;
	owner = nullptr;
}
Spell::Spell(eFaction _faction, eTarget _target, int _cost, std::string path, std::string _name, Effect effect) : Spell(_faction, _target, _cost, path, _name) {
	if (effect.effect != EFFECT_NONE)
		AddEffect(effect, this);
}
Spell::~Spell() {}

//Generate sidebar details
void Spell::GenerateDetails() {
	std::string s;
	s.resize(name.size());
	std::transform(name.begin(), name.end(), s.begin(), ::toupper);
	s += " - SPELL";
	header[0].CreateFromString(s);
	for (int i = name.size(); i < header[0].buffer.size(); ++i)
		header[0].buffer[i].Attributes = COLOR_GRAY;
	UpdateDetailStats();
}

#pragma endregion

#pragma region Rendering

//Draw card details
void Spell::DrawDetails(Renderer& renderer, int& y) {
	sprite.SetColor(COLOR_LTWHITE);
	renderer.Render(sprite, 66, y);
	renderer.Render(header[0], 72, y); ++y;
	UpdateDetailStats();
	renderer.Render(header[1], 72, y); y += 2;
	for (int i = 0; i < effects.size(); ++i) {
		renderer.Render(effects[i].sprite, 72, y);
		y += effects[i].sprite.height + 1;
	}
	if (token != nullptr) {
		if (y < 7) { y = 7; }
		renderer.Render(divider, 66, y);
		y += 2;
		token->DrawDetails(renderer, y);
	}
}

#pragma endregion

#pragma region Updates

//Update cost when buffs change
void Spell::UpdateStatBuffs() {
	int costBuff = 0;
	for (int i = 0; i < effects.size(); ++i)
		costBuff += effects[i].costBuff * effects[i].sources.size();
	cost = max(original->cost + costBuff, 0);
}

//Update card stats
void Spell::UpdateDetailStats() {
	std::string s = "COST:" + std::to_string(cost);
	header[1].CreateFromString(s);
	for (int i = 0; i < header[1].buffer.size(); ++i)
		if (s[i] != ':')
			header[1].buffer[i].Attributes = COLOR_LTBLUE;
}

#pragma endregion

#pragma region Events

//When a card is cast
void Spell::OnCast(Card* card, BoardTile* tile) {

	//Trigger any effects on this card
	Card::OnCast(card, tile);

	//Cast this
	if (card == this)
		game->grave.push_back(this);

}

//On card use
void Spell::OnUse(BoardTile* tile) {

	/*

	switch (spell.spell) {
	case SPELL_CONSUMING_REBIRTH:
		if (tile->minion != nullptr) {
			token = tile->minion->original;
			tile->minion->isDead = true;
			game->eventManager.SendOnDeath(tile->minion);
			game->lateCallback.push_back(EffectCallback(nullptr, this, tile->minion->curTile, SKILL_NONE));
		}
		game->eventManager.SendOnSpellCast(this);
		break;
	case SPELL_DAEMONIC_LURE:
		game->HighlightSelectable(TARGET_TILE);
		if (game->selectable.size() > 0) {
			game->callback = EffectCallback(nullptr, this, nullptr, SKILL_NONE);
			token = tile->minion;
		}
		else { tile->minion->DealDamage(nullptr, 1); }
		break;
	case SPELL_DARK_TRANSFORMATION:
		if (true) { //Need this to initialize variable
			Minion* u1 = tile->minion;
			if (u1 != nullptr) {
				tile->minion->isDead = true;
				Minion* u2 = new Minion(*(dynamic_cast<Minion*>(token)));
				game->SetContext(u2, owner);
				game->Summon(u2, tile->pos.x, tile->pos.y, false);
				game->eventManager.SendOnDeath(u1);
			}
		}
		game->eventManager.SendOnSpellCast(this);
		break;
	case SPELL_DARKFIRE_SACRIFICE:
		if (tile->minion != nullptr) {
			tile->minion->isDead = true;
			game->eventManager.SendOnDeath(tile->minion);
			owner->general->AddEffect(EFFECT_DARKFIRE_SACRIFICE);
			for (int a = 0; a < owner->hand.size(); ++a) {
				if (owner->hand[a]->cardType == CARD_UNIT) {
					dynamic_cast<Minion*>(owner->hand[a])->AddBuff(BUFF_DARKFIRE_SACRIFICE);
				}
			}
			for (int a = 0; a < owner->deck.size(); ++a) {
				if (owner->deck[a]->cardType == CARD_UNIT) {
					dynamic_cast<Minion*>(owner->deck[a])->AddBuff(BUFF_DARKFIRE_SACRIFICE);
				}
			}
		}
		game->eventManager.SendOnSpellCast(this);
		break;
	}

	*/

}

//Immediate callback
void Spell::Callback(BoardTile* tile) {

	/*

	switch (spell.spell) {
	case SPELL_DAEMONIC_LURE:
		Minion* u = dynamic_cast<Minion*>(token);
		u->SetPosition(tile->pos.x, tile->pos.y);
		u->DealDamage(nullptr, 1);
		game->eventManager.SendOnMove(u, true);
		game->eventManager.SendOnSpellCast(this);
		break;
	}
	game->callback = EffectCallback();

	*/

}

//Late callback
void Spell::LateCallback() {

	/*

	switch (spell.spell) {
	case SPELL_CONSUMING_REBIRTH:
		if (game->lateCallback[0].tile->minion == nullptr) {
			Minion* t = new Minion(*(dynamic_cast<Minion*>(token)));
			game->SetContext(t, owner);
			game->Summon(t, game->lateCallback[0].tile->pos.x, game->lateCallback[0].tile->pos.y, false);
			game->minions.back()->AddBuff(BUFF_CONSUMING_REBIRTH);
		}
		break;
	}

	*/

}

#pragma endregion
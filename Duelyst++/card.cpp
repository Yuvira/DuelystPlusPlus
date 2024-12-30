//Include
#include "game.h"

#pragma region Card Constructor

//Card constructor
Card::Card() {
	faction = FACTION_NEUTRAL;
	cardType = CARD_NONE;
	isToken = false;
	cost = 0;
	game = nullptr;
	owner = nullptr;
	original = this;
	token = nullptr;
	name = "???";
	divider.CreateFromString("컴TOKEN컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴");
}
Card::~Card() {}

#pragma endregion

#pragma region Card Effects

//Add effect to list
void Card::AddEffect(Effect effect, Card* source) {
	for (int i = 0; i < effects.size(); ++i) {
		if (effects[i].effect == effect.effect) {
			effects[i].sources.push_back(source);
			effects[i].GenerateSprite();
			UpdateStatBuffs();
			return;
		}
	}
	effects.push_back(effect);
	effects.back().sources.push_back(source);
	effects.back().GenerateSprite();
	UpdateStatBuffs();
}

//Remove effect from list
void Card::RemoveEffect(Effect effect, Card* source, bool allStacks) {
	for (int i = 0; i < effects.size(); ++i) {
		if (effects[i].effect == effect.effect) {
			for (int j = 0; j < effects[i].sources.size(); ++j) {
				if (effects[i].sources[j] == source) {
					effects[i].sources.erase(effects[i].sources.begin() + j);
					if (!allStacks)
						break;
					--j;
				}
			}
			if (effects[i].sources.size() == 0) {
				effects.erase(effects.begin() + i);
				break;
			}
		}
	}
	UpdateStatBuffs();
}

#pragma endregion

#pragma region Events

//When this is cast (before minion is summoned or spell effects occur)
void Card::PreCast(BoardTile* tile) {
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnPreCastThis)
			effects[i].OnPreCastThis(this, tile);
}

//Whenever any card is cast
void Card::OnCast(Card* card, BoardTile* tile) {
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnCast)
			effects[i].OnCast(this, card, tile);
}

//Whenever a minion is summoned
void Card::OnSummon(Minion* minion, bool fromActionBar) {
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnSummon)
			effects[i].OnSummon(this, minion, fromActionBar);
}

//Whenever a minion dies
void Card::OnDeath(Minion* minion) {
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnDeath)
			effects[i].OnDeath(this, minion);
}

//Whenever a minion dies
void Card::OnTurnEnd(Player* player) {
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnTurnEnd)
			effects[i].OnTurnEnd(this, player);
}

//Whenever a minion dies
void Card::OnTurnStart(Player* player) {
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnTurnStart)
			effects[i].OnTurnStart(this, player);
}

#pragma endregion
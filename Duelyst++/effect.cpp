//Include
#include "game.h"

#pragma region Constructors

//Self reference constructor
EffectContext::EffectContext() : EffectContext(nullptr, nullptr, nullptr) {}
EffectContext::EffectContext(Effect* effect, Card* card, Game* game) {
	this->effect = effect;
	this->card = card;
	this->game = game;
}
EffectContext::~EffectContext() {}

//Effect constructors
Effect::Effect() : Effect(EFFECT_NONE, KEYWORD_NONE, "") {}
Effect::Effect(eEffect effect, int keywords, std::string description) {
	this->effect = effect;
	this->keywords = keywords;
	this->description = description;
	triggered = false;
	fixedCost = -1;
	costBuff = 0;
	atkBuff = 0;
	hpBuff = 0;
	moveBuff = 0;
	token = nullptr;
	source = nullptr;
}
Effect::~Effect() {}

#pragma endregion

#pragma region Utils

//Utilities
bool EffectContext::IsOnBoard() { return card->IsOnBoard(); }
bool EffectContext::BothOnBoard(Card* compare) { return card->IsOnBoard() && compare->IsOnBoard(); }
bool EffectContext::IsCard(Card* compare) { return card == compare; }
bool EffectContext::IsCardOnBoard(Card* compare) { return card->IsOnBoard() && card == compare; }
bool EffectContext::SharesOwner(Card* compare) { return card->owner == compare->owner; }
bool EffectContext::IsOwnedBy(Player* compare) { return card->owner == compare; }
bool EffectContext::IsAllied(Card* compare) { return card != compare && card->owner == compare->owner; }
bool EffectContext::IsOwnerTurn() { return &game->players[game->turn] == card->owner; }

#pragma endregion
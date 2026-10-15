//Include
#include "effect.h"
#include "game.h"
#include "card.h"

//Effect constructors
Effect::Effect() : Effect(EFFECT_NONE, KEYWORD_NONE, "") {}
Effect::Effect(eEffect effectId, int keywords, std::string description) {
	this->effectId = effectId;
	this->keywords = keywords;
	this->description = description;
	triggered = false;
	fixedCost = -1;
	costBuff = 0;
	atkBuff = 0;
	hpBuff = 0;
	moveBuff = 0;
	token = nullptr;
	refCard = nullptr;
	source = nullptr;
}
Effect::~Effect() {}

//Trigger events
void Effect::TriggerPreEvent(EffectContext context, Event* event) {
	if (OnPreEvent)
		OnPreEvent(context, event);
}
void Effect::TriggerPostEvent(EffectContext context, Event* event) {
	if (OnPostEvent)
		OnPostEvent(context, event);
}

//Trigger legacy events
void Effect::TriggerOnAddThis(EffectContext context) {
	if (OnAddThis)
		OnAddThis(context);
}
void Effect::TriggerOnRemoveThis(EffectContext context) {
	if (OnRemoveThis)
		OnRemoveThis(context);
}
void Effect::TriggerOnPreCastThis(EffectContext context, BoardTile* tile) {
	if (OnPreCastThis)
		OnPreCastThis(context, tile);
}
void Effect::TriggerOnResolveThis(EffectContext context, BoardTile* tile) {
	if (OnResolveThis)
		OnResolveThis(context, tile);
}

//Getters
Card* Effect::GetToken() { return token; }
const eEffect& Effect::GetID() const { return effectId; }
const int& Effect::GetKeywords() const { return keywords; }
const std::string& Effect::GetDescription() const { return description; }
const int& Effect::GetCostBuff() const { return costBuff; }
const int& Effect::GetFixedCost() const { return fixedCost; }
const int& Effect::GetAtkBuff() const { return atkBuff; }
const int& Effect::GetHPBuff() const { return hpBuff; }
const int& Effect::GetMoveBuff() const { return moveBuff; }
const Effect* const& Effect::GetSource() const { return source; };
Card* Effect::GetRef() { return refCard; };

//Setters
void Effect::SetCostBuff(int costBuff) { this->costBuff = costBuff; }
void Effect::SetFixedCost(int fixedCost) { this->fixedCost = fixedCost; }
void Effect::SetAtkBuff(int atkBuff) { this->atkBuff = atkBuff; }
void Effect::SetHPBuff(int hpBuff) { this->hpBuff = hpBuff; }
void Effect::SetMoveBuff(int moveBuff) { this->moveBuff = moveBuff; }
void Effect::SetToken(Card* token) { this->token = token; }
void Effect::SetSources(Effect* source, Card* refCard) {
	this->source = source;
	this->refCard = refCard;
}

//Utilities
bool EffectContext::IsOnBoard() { return card->IsOnBoard(); }
bool EffectContext::BothOnBoard(Card* compare) { return card->IsOnBoard() && compare->IsOnBoard(); }
bool EffectContext::IsCard(Card* compare) { return card == compare; }
bool EffectContext::IsCardOnBoard(Card* compare) { return card->IsOnBoard() && card == compare; }
bool EffectContext::SharesOwner(Card* compare) { return card->owner == compare->owner; }
bool EffectContext::IsOwnedBy(Player* compare) { return card->owner == compare; }
bool EffectContext::IsAllied(Card* compare) { return card != compare && card->owner == compare->owner; }
bool EffectContext::IsOwnerTurn() { return game->GetTurnPlayer() == card->owner; }
//Include
#include "effect.h"
#include "game.h"
#include "card.h"
#include "tile.h"

//Effect constructors
Effect::Effect() : Effect(EFFECT_NONE, KEYWORD_NONE, "") {}
Effect::Effect(eEffect effectId, int keywords, std::string description) {
	this->effectId = effectId;
	this->keywords = keywords;
	this->description = description;
	token = nullptr;
	refCard = nullptr;
	source = nullptr;
}
Effect::~Effect() {}

//Dispatch triggers
void Effect::DispatchTrigger(const std::vector<EventTrigger>& triggers, EffectContext ctx, Event* ev) {
	for (const EventTrigger& trigger : triggers)
		if (trigger.type == ev->type)
			trigger.Invoke(trigger, ctx, ev);
}

//Trigger events
void Effect::TriggerPreEvent(EffectContext ctx, Event* ev) { DispatchTrigger(preTriggers, ctx, ev); }
void Effect::TriggerPostEvent(EffectContext ctx, Event* ev) { DispatchTrigger(postTriggers, ctx, ev); }
void Effect::TriggerSelfEvent(EffectContext ctx, Event* ev) { DispatchTrigger(selfTriggers, ctx, ev); }

//Modifier updates
Effect& Effect::AddModifier(eStat stat, eModifier type, int value) {
	if (!modifiers.AddModifier(stat, type, value))
		description = "AddModifier Error";
	return *this;
}
void Effect::UpdateModifier(eStat stat, int value) {
	if (!modifiers.UpdateModifier(stat, value))
		description = "UpdateModifier Error";
}

//Getters & Setters
Card* Effect::GetToken() { return token; }
Card* Effect::GetRef() { return refCard; };
void Effect::SetToken(Card* token) { this->token = token; }
void Effect::SetSources(Effect* source, Card* refCard) {
	this->source = source;
	this->refCard = refCard;
}

//Readonly getters
const eEffect& Effect::GetID() const { return effectId; }
const std::string& Effect::GetDescription() const { return description; }
const int& Effect::GetKeywords() const { return keywords; }
const ModifierGroup& Effect::GetModifiers() const { return modifiers; }
const Effect* const& Effect::GetSource() const { return source; };

//Utilities
bool EffectContext::IsOnBoard() { return card->IsOnBoard(); }
bool EffectContext::BothOnBoard(Card* compare) { return card->IsOnBoard() && compare->IsOnBoard(); }
bool EffectContext::IsCard(Card* compare) { return card == compare; }
bool EffectContext::IsCardOnBoard(Card* compare) { return card->IsOnBoard() && card == compare; }
bool EffectContext::SharesOwner(Card* compare) { return card->owner == compare->owner; }
bool EffectContext::IsOwnedBy(Player* compare) { return card->owner == compare; }
bool EffectContext::IsAllied(Card* compare) { return card != compare && card->owner == compare->owner; }
bool EffectContext::IsOwnerTurn() { return game->GetTurnPlayer() == card->owner; }
Player* EffectContext::GetOwner() { return card->owner; }
Player* EffectContext::GetOpponent() { return card->owner->opponent; }
Minion* EffectContext::GetMinion() { return card->GetMinion(); }
Minion* EffectContext::GetAllyGeneral() { return card->owner->general; }
Minion* EffectContext::GetEnemyGeneral() { return card->owner->opponent->general; }
BoardTile* EffectContext::GetTile() { return card->IsOnBoard() ? card->GetMinion()->curTile : nullptr; }
Map* EffectContext::GetMap() { return &game->map; }
//Include
#include "card.h"
#include "effect.h"
#include "effectdefinition.h"
#include "game.h"
#include "tile.h"

//Effect constructor
Effect::Effect(const EffectDefinition& definition, Card* sourceCard, Effect* sourceEffect) {
	this->effectId = definition.effectId;
	this->description = definition.description;
	this->keywords = definition.keywords;
	this->modifiers = definition.modifiers;
	this->token = definition.token;
	this->isRemovable = definition.isRemovable;
	this->sourceCard = sourceCard;
	this->sourceEffect = sourceEffect;
	AuraPredicate = definition.AuraPredicate;
	preTriggers = definition.preTriggers;
	postTriggers = definition.postTriggers;
	selfTriggers = definition.selfTriggers;
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
void Effect::UpdateModifier(eStat stat, int value) {
	if (!modifiers.UpdateModifier(stat, value))
		description = "UpdateModifier Error";
}

//EffectContext Utilities
bool EffectContext::IsOnBoard() { return card->IsOnBoard(); }
bool EffectContext::BothOnBoard(Card* compare) { return card->IsOnBoard() && compare->IsOnBoard(); }
bool EffectContext::IsCard(Card* compare) { return card == compare; }
bool EffectContext::IsCardOnBoard(Card* compare) { return card->IsOnBoard() && card == compare; }
bool EffectContext::SharesOwner(Card* compare) { return card->owner == compare->owner; }
bool EffectContext::IsOwnedBy(Player* compare) { return card->owner == compare; }
bool EffectContext::IsAllied(Card* compare) { return card != compare && card->owner == compare->owner; }
bool EffectContext::IsOwnerTurn() { return game->GetTurnPlayer() == card->owner; }
bool EffectContext::CardIsAuraSource() { return card == effect->GetSourceCard(); }
bool EffectContext::CardSharesAuraOwner() { return card->owner == effect->GetSourceCard()->owner; }
Player* EffectContext::GetOwner() { return card->owner; }
Player* EffectContext::GetOpponent() { return card->owner->opponent; }
Unit* EffectContext::GetUnit() { return card->GetUnit(); }
Unit* EffectContext::GetAllyGeneral() { return card->owner->general; }
Unit* EffectContext::GetEnemyGeneral() { return card->owner->opponent->general; }
BoardTile* EffectContext::GetTile() { return card->IsOnBoard() ? card->GetUnit()->curTile : nullptr; }
Map* EffectContext::GetMap() { return &game->map; }
//Include
#include "card.h"
#include "effect.h"
#include "effectdefinition.h"
#include "game.h"
#include "tile.h"

//Effect constructors
Effect::Effect(const EffectDefinition& definition) {
	this->effectId = definition.effectId;
	this->description = definition.description;
	this->keywords = definition.keywords;
	this->modifiers = definition.modifiers;
	this->token = definition.token;
	this->isRemovable = definition.isRemovable;
	preTriggers = definition.preTriggers;
	postTriggers = definition.postTriggers;
	selfTriggers = definition.selfTriggers;
}
Effect::~Effect() {}
Aura::Aura(const EffectDefinition& definition, Player* owner, Card* sourceCard, Effect* sourceEffect) : Effect(definition) {
	this->owner = owner;
	this->sourceCard = sourceCard;
	this->sourceEffect = sourceEffect;
	Predicate = definition.AuraPredicate;
}
Aura::~Aura() {}
SpellEffect::SpellEffect(const EffectDefinition& definition) : Effect(definition) {
	TargetMethod = definition.TargetMethod;
}
SpellEffect::~SpellEffect() {}

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

//Aura owner
Player* Aura::GetOwner() { return Predicate ? sourceCard->owner : owner; }

//EffectContext Utilities
bool EffectContext::IsOnBoard() { return card->IsOnBoard(); }
bool EffectContext::BothOnBoard(Card* compare) { return card->IsOnBoard() && compare->IsOnBoard(); }
bool EffectContext::IsCard(Card* compare) { return card == compare; }
bool EffectContext::IsCardOnBoard(Card* compare) { return card->IsOnBoard() && card == compare; }
bool EffectContext::IsInOwnerHand() { return card->owner->HasCardInHand(card); }
bool EffectContext::SharesOwner(Card* compare) { return card->owner == compare->owner; }
bool EffectContext::IsOwnedBy(Player* compare) { return card->owner == compare; }
bool EffectContext::IsAllied(Card* compare) { return card != compare && card->owner == compare->owner; }
bool EffectContext::IsOwnerTurn() { return game->GetTurnPlayer() == card->owner; }
bool EffectContext::CardIsAuraSource() { return effect->IsAura() && card == effect->GetAura()->GetSourceCard(); }
bool EffectContext::CardSharesAuraOwner() { return effect->IsAura() && card->owner == effect->GetAura()->GetOwner(); }
bool EffectContext::CardIsFriendOfAuraSource() { return effect->IsAura() && card->owner == effect->GetAura()->GetSourceCard()->owner && card != effect->GetAura()->GetSourceCard(); }
Player* EffectContext::GetOwner() { return card->owner; }
Player* EffectContext::GetOpponent() { return card->owner->opponent; }
Unit* EffectContext::GetUnit() { return card->GetUnit(); }
Unit* EffectContext::GetAllyGeneral() { return card->owner->general; }
Unit* EffectContext::GetEnemyGeneral() { return card->owner->opponent->general; }
BoardTile* EffectContext::GetTile() { return card->IsOnBoard() ? card->GetUnit()->curTile : nullptr; }
Map* EffectContext::GetMap() { return &game->map; }
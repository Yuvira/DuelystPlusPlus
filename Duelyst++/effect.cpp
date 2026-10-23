//Include
#include "card.h"
#include "effect.h"
#include "effectcontext.h"
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
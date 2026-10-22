#pragma once

//Include
#include <string>
#include <vector>
#include "cardids.h"
#include "effectids.h"
#include "modifiers.h"

//Declaration
class EffectContext;
class EventTrigger;

//Effect Definition
struct EffectDefinition {

	//Constructor
	EffectDefinition(eEffect effectId, int keywords, std::string description) {
		this->effectId = effectId;
		this->keywords = keywords;
		this->description = description;
		token = CARD_NONE;
		AuraPredicate = nullptr;
	}

	//Trigger registers
	template <typename T>
	void RegisterPreTrigger(bool (*Predicate)(EffectContext ctx, T& ev), void (*Trigger)(EffectContext ctx, T& ev)) {
		preTriggers.emplace_back(Predicate, Trigger);
	}
	template <typename T>
	void RegisterPostTrigger(bool (*Predicate)(EffectContext ctx, T& ev), void (*Trigger)(EffectContext ctx, T& ev)) {
		postTriggers.emplace_back(Predicate, Trigger);
	}
	template <typename T>
	void RegisterSelfTrigger(void (*Trigger)(EffectContext ctx, T& ev)) {
		selfTriggers.emplace_back(static_cast<bool (*)(EffectContext ctx, T & ev)>(nullptr), Trigger);
	}

	//Additional setters
	EffectDefinition& AddModifier(eStat stat, eModifier type, int value) {
		if (!modifiers.AddModifier(stat, type, value))
			description = "AddModifier Error";
		return *this;
	}
	void HideModifiers() { modifiers.HideValues(); }
	void SetToken(eCard token) { this->token = token; }
	void SetAuraPredicate(bool (*Predicate)(EffectContext ctx)) { AuraPredicate = Predicate; }

	//Properties
	eEffect effectId;
	std::string description;
	int keywords;
	ModifierGroup modifiers;
	eCard token;
	bool (*AuraPredicate)(EffectContext ctx);
	std::vector<EventTrigger> preTriggers;
	std::vector<EventTrigger> postTriggers;
	std::vector<EventTrigger> selfTriggers;

};
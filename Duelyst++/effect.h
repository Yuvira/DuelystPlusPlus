#pragma once

//Include
#include <string>
#include <vector>
#include "effectcontext.h"
#include "effectids.h"
#include "event.h"
#include "modifiers.h"

//Definitions
class Aura;
class BoardTile;
class Card;
class Effect;
class EffectDefinition;
class Game;
class Map;
class Player;
class Spell;
class SpellEffect;
enum eCard;

#pragma region Helper Structs

//Event trigger
struct EventTrigger {

	//Constructor
	template <typename T>
	EventTrigger(bool (*Predicate)(EffectContext ctx, T& ev), void (*Trigger)(EffectContext ctx, T& ev)) :
		type(T::typeKey),
		Invoke(&InvokeTrigger<T>),
		Predicate(reinterpret_cast<void (*)()>(Predicate)),
		Trigger(reinterpret_cast<void (*)()>(Trigger)) {}

	//Invoke method
	template <typename T>
	static void InvokeTrigger(const EventTrigger& trigger, EffectContext ctx, Event* ev) {
		T& typedEv = *static_cast<T*>(ev);
		bool (*Predicate)(EffectContext ctx, T & ev) = reinterpret_cast<bool (*)(EffectContext ctx, T& ev)>(trigger.Predicate);
		void (*Trigger)(EffectContext ctx, T & ev) = reinterpret_cast<void (*)(EffectContext ctx, T& ev)>(trigger.Trigger);
		if (!Predicate || Predicate(ctx, typedEv))
			Trigger(ctx, typedEv);
	}

	//Properties
	eEventType type;
	void (*Invoke)(const EventTrigger& trigger, EffectContext ctx, Event* ev);
	void (*Predicate)();
	void (*Trigger)();

};

#pragma endregion

//Effect class
class Effect {

public:

	//Constructors
	Effect(const EffectDefinition& definition);
	~Effect();

	//Trigger dispatch
	static void DispatchTrigger(const std::vector<EventTrigger>& triggers, EffectContext ctx, Event* ev);
	void TriggerPreEvent(EffectContext ctx, Event* ev);
	void TriggerPostEvent(EffectContext ctx, Event* ev);
	void TriggerSelfEvent(EffectContext ctx, Event* ev);

	//Setters
	void UpdateModifier(eStat stat, int value);
	void SetToken(eCard token) { this->token = token; }

	//Readonly getters
	const eEffect& ReadID() const { return effectId; }
	const std::string& ReadDescription() const { return description; }
	const int& ReadKeywords() const { return keywords; }
	const eCard& ReadToken() const { return token; }
	const ModifierGroup& ReadModifiers() const { return modifiers; }
	const bool& ReadIsRemovable() const { return isRemovable; }

	//Subclass getters
	virtual Aura* GetAura() { return nullptr; }
	virtual SpellEffect* GetSpellEffect() { return nullptr; }
	bool IsAura() { return GetAura() != nullptr; }
	bool IsSpellEffect() { return GetSpellEffect() != nullptr; }

protected:

	//Properties
	eEffect effectId;
	std::string description;
	int keywords;
	ModifierGroup modifiers;
	eCard token;
	bool isRemovable;
	std::vector<EventTrigger> preTriggers;
	std::vector<EventTrigger> postTriggers;
	std::vector<EventTrigger> selfTriggers;

};

//Aura that takes pointers to source objects and a predicate
class Aura : public Effect {

public:

	//Constructor
	Aura(const EffectDefinition& definition, Player* owner, Card* sourceCard, Effect* sourceEffect);
	~Aura();

	//Utils
	bool MatchesPredicate(Game* game, Card* card) { return Predicate && Predicate(EffectContext(this, card, game)); }

	//Source getters
	Player* GetOwner();
	Card* GetSourceCard() { return sourceCard; };
	Effect* GetSourceEffect() { return sourceEffect; };

	//Subclass getter
	Aura* GetAura() { return this; }

private:

	//Properties
	Player* owner;
	Card* sourceCard;
	Effect* sourceEffect;
	bool (*Predicate)(EffectContext ctx);

};

//Spell effect that has a built-in targeting mode
class SpellEffect : public Effect {

public:

	//Constructor
	SpellEffect(const EffectDefinition& definition);
	~SpellEffect();

	//Initial targeting
	std::vector<BoardTile*> GetTargets(Game* game, Spell* spell) {
		std::vector<BoardTile*> empty;
		if (TargetMethod)
			return TargetMethod(game, spell);
		return empty;
	};

	//Effect targets
	void AddTarget(BoardTile* tile) { effectTargets.push_back(tile); }
	std::vector<BoardTile*> GetEffectTargets() { return effectTargets; }

	//Subclass getter
	SpellEffect* GetSpellEffect() { return this; }

private:

	//Target method
	std::vector<BoardTile*> effectTargets;
	std::vector<BoardTile*> (*TargetMethod)(Game* game, Spell* spell);

};
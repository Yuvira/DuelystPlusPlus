#pragma once

//Include
#include <string>
#include <vector>
#include "effectids.h"
#include "event.h"
#include "modifiers.h"

//Definitions
class Card;
class BoardTile;
class Effect;
class EffectDefinition;
class Game;
class Map;
class Player;
enum eCard;

#pragma region Helper Structs

//Self reference container for passing to lambdas
struct EffectContext {

	//Constructor
	EffectContext() : EffectContext(nullptr, nullptr, nullptr) {}
	EffectContext(Effect* effect, Card* card, Game* game) {
		this->effect = effect;
		this->card = card;
		this->game = game;
	}

	//Helpers
	bool IsOnBoard();
	bool BothOnBoard(Card* compare);
	bool IsCard(Card* compare);
	bool IsCardOnBoard(Card* compare);
	bool SharesOwner(Card* compare);
	bool IsOwnedBy(Player* compare);
	bool IsAllied(Card* compare);
	bool IsOwnerTurn();
	bool CardIsAuraSource();
	bool CardSharesAuraOwner();
	Player* GetOwner();
	Player* GetOpponent();
	Unit* GetUnit();
	Unit* GetAllyGeneral();
	Unit* GetEnemyGeneral();
	BoardTile* GetTile();
	Map* GetMap();

	//Properties
	Effect* effect;
	Card* card;
	Game* game;

};

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

	//Utils
	void UpdateModifier(eStat stat, int value);
	bool MatchesAuraPredicate(Game* game, Card* card) { return AuraPredicate && AuraPredicate(EffectContext(this, card, game)); }

	//Getters & Setters
	Card* GetSourceCard() { return sourceCard; };
	Effect* GetSourceEffect() { return sourceEffect; };
	void SetToken(eCard token) { this->token = token; }
	void SetSources(Card* card, Effect* effect) {
		this->sourceCard = card;
		this->sourceEffect = effect;
	}

	//Readonly getters
	const eEffect& ReadID() const { return effectId; }
	const std::string& ReadDescription() const { return description; }
	const int& ReadKeywords() const { return keywords; }
	const eCard& ReadToken() const { return token; }
	const ModifierGroup& ReadModifiers() const { return modifiers; }

private:

	//Properties
	eEffect effectId;
	std::string description;
	int keywords;
	ModifierGroup modifiers;
	eCard token;
	Card* sourceCard;
	Effect* sourceEffect;
	bool (*AuraPredicate)(EffectContext ctx);
	std::vector<EventTrigger> preTriggers;
	std::vector<EventTrigger> postTriggers;
	std::vector<EventTrigger> selfTriggers;

};
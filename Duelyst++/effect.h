#pragma once

//Include
#include <string>
#include <vector>
#include "effectids.h"
#include "event.h"
#include "modifiers.h"

//Definitions
class Game;
class BoardTile;
class Map;
class Player;
class Card;
class Effect;

#pragma region Helper Structs

//Self reference container for passing to lambdas
struct EffectContext {
	EffectContext(Effect* effect, Card* card, Game* game) {
		this->effect = effect;
		this->card = card;
		this->game = game;
	}
	bool IsOnBoard();
	bool BothOnBoard(Card* compare);
	bool IsCard(Card* compare);
	bool IsCardOnBoard(Card* compare);
	bool SharesOwner(Card* compare);
	bool IsOwnedBy(Player* compare);
	bool IsAllied(Card* compare);
	bool IsOwnerTurn();
	Player* GetOwner();
	Player* GetOpponent();
	Unit* GetUnit();
	Unit* GetAllyGeneral();
	Unit* GetEnemyGeneral();
	BoardTile* GetTile();
	Map* GetMap();
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
	Effect();
	Effect(eEffect effectId, int keywords, std::string description);
	~Effect();

	//Trigger registration
	template <typename T>
	void RegisterPreTrigger(bool (*Predicate)(EffectContext ctx, T& ev), void (*Trigger)(EffectContext ctx, T& ev)) {
		preTriggers.emplace_back(Predicate, Trigger); }
	template <typename T>
	void RegisterPostTrigger(bool (*Predicate)(EffectContext ctx, T& ev), void (*Trigger)(EffectContext ctx, T& ev)) {
		postTriggers.emplace_back(Predicate, Trigger); }
	template <typename T>
	void RegisterSelfTrigger(void (*Trigger)(EffectContext ctx, T& ev)) {
		selfTriggers.emplace_back(static_cast<bool (*)(EffectContext ctx, T& ev)>(nullptr), Trigger); }

	//Trigger dispatch
	static void DispatchTrigger(const std::vector<EventTrigger>& triggers, EffectContext ctx, Event* ev);
	void TriggerPreEvent(EffectContext ctx, Event* ev);
	void TriggerPostEvent(EffectContext ctx, Event* ev);
	void TriggerSelfEvent(EffectContext ctx, Event* ev);

	//Modifier updates
	Effect& AddModifier(eStat stat, eModifier type, int value);
	void UpdateModifier(eStat stat, int value);

	//Aura predicate
	void SetAuraPredicate(bool (*Predicate)(Effect* aura, Game* game, Card* card)) { AuraPredicate = Predicate; }
	bool MatchesAuraPredicate(Game* game, Card* card) { return AuraPredicate && AuraPredicate(this, game, card); }

	//Getters & Setters
	Card* GetToken();
	Card* GetRef();
	Effect* GetSource();
	void SetToken(Card* token);
	void SetSources(Effect* source, Card* refCard);

	//Readonly getters
	const eEffect& GetID() const;
	const std::string& GetDescription() const;
	const int& GetKeywords() const;
	const ModifierGroup& GetModifiers() const;

	//Utils
	bool IsContinuous() { return source != nullptr; }

private:

	//Properties
	eEffect effectId;
	std::string description;
	int keywords;
	ModifierGroup modifiers;
	Card* token;
	Card* refCard;
	Effect* source;
	bool (*AuraPredicate)(Effect* aura, Game* game, Card* card);
	std::vector<EventTrigger> preTriggers;
	std::vector<EventTrigger> postTriggers;
	std::vector<EventTrigger> selfTriggers;

};
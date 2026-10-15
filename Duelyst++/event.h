#pragma once

//Type enum
enum eEventType {
	EVENT_NONE,
	EVENT_CAST,
	EVENT_SUMMON,
	EVENT_DEATH,
	EVENT_ATTACK,
	EVENT_DAMAGE,
	EVENT_HEAL,
	EVENT_MOVE,
	EVENT_DRAW,
	EVENT_ADD_TO_HAND,
	EVENT_REPLACE,
	EVENT_EFFECTS_CHANGED,
	EVENT_TURN_END,
	EVENT_TURN_START
};

//Define
class Player;
class BoardTile;
class Card;
class Minion;

//Event object
struct Event {
	Event(int turn) {
		type = EVENT_NONE;
		this->turn = turn;
	}
	template <typename T> T* As() { return type == T::typeKey ? static_cast<T*>(this) : nullptr; }
	template <typename T> const T* As() const { return type == T::typeKey ? static_cast<const T*>(this) : nullptr; }
	eEventType type;
	int turn;
};

//Triggers whenever a player casts a card from their action bar
struct OnCastEvent : Event {
	static constexpr eEventType typeKey = EVENT_CAST;
	OnCastEvent(int turn, Card* card, BoardTile* tile) : Event(turn) {
		type = EVENT_CAST;
		this->card = card;
		this->tile = tile;
	}
	Card* card;
	BoardTile* tile;
};

//Triggers whenever a unit is summoned to the board
struct OnSummonEvent : Event {
	static constexpr eEventType typeKey = EVENT_SUMMON;
	OnSummonEvent(int turn, Minion* minion, bool wasCast) : Event(turn) {
		type = EVENT_SUMMON;
		this->minion = minion;
		this->wasCast = wasCast;
	}
	Minion* minion;
	bool wasCast;
};

//Triggers whenever a unit dies
struct OnDeathEvent : Event {
	static constexpr eEventType typeKey = EVENT_DEATH;
	OnDeathEvent(int turn, Minion* minion) : Event(turn) {
		type = EVENT_DEATH;
		this->minion = minion;
	}
	Minion* minion;
};

//Triggers whenever a unit attacks another unit
struct OnAttackEvent : Event {
	static constexpr eEventType typeKey = EVENT_ATTACK;
	OnAttackEvent(int turn, Minion* source, Minion* target, bool isCounterattack) : Event(turn) {
		type = EVENT_ATTACK;
		this->source = source;
		this->target = target;
		this->isCounterattack = isCounterattack;
	}
	Minion* source;
	Minion* target;
	bool isCounterattack;
};

//Triggers whenever a unit is dealt damage
struct OnDamageEvent : Event {
	static constexpr eEventType typeKey = EVENT_DAMAGE;
	OnDamageEvent(int turn, Card* source, Minion* target, int damage) : Event(turn) {
		type = EVENT_DAMAGE;
		this->source = source;
		this->target = target;
		this->damage = damage;
	}
	Card* source;
	Minion* target;
	int damage;
};

//Triggers whenever a unit is healed
struct OnHealEvent : Event {
	static constexpr eEventType typeKey = EVENT_HEAL;
	OnHealEvent(int turn, Card* source, Minion* target, int heal) : Event(turn) {
		type = EVENT_HEAL;
		this->source = source;
		this->target = target;
		this->heal = heal;
	}
	Card* source;
	Minion* target;
	int heal;
};

//Triggers whenever a unit moves or is moved
struct OnMoveEvent : Event {
	static constexpr eEventType typeKey = EVENT_MOVE;
	OnMoveEvent(int turn, Minion* minion, bool wasTeleported) : Event(turn) {
		type = EVENT_MOVE;
		this->minion = minion;
		this->wasTeleported = wasTeleported;
	}
	Minion* minion;
	bool wasTeleported;
};

//Triggers whenever a player draws a card from their deck
struct OnDrawEvent : Event {
	static constexpr eEventType typeKey = EVENT_DRAW;
	OnDrawEvent(int turn, Card* card) : Event(turn) {
		type = EVENT_DRAW;
		this->card = card;
	}
	Card* card;
};

//Triggers whenever a player has a card added to their hand from somewhere other than their deck
struct OnAddToHandEvent : Event {
	static constexpr eEventType typeKey = EVENT_ADD_TO_HAND;
	OnAddToHandEvent(int turn, Card* card) : Event(turn) {
		type = EVENT_ADD_TO_HAND;
		this->card = card;
	}
	Card* card;
};

//Triggers whenever a player replaces a card in their hand
struct OnReplaceEvent : Event {
	static constexpr eEventType typeKey = EVENT_REPLACE;
	OnReplaceEvent(int turn, Card* card, bool sendToDeck) : Event(turn) {
		type = EVENT_REPLACE;
		this->card = card;
		this->sendToDeck = sendToDeck;
	}
	Card* card;
	bool sendToDeck;
};

//Triggers whenever the effects on a card change
struct OnEffectsChangedEvent : Event {
	static constexpr eEventType typeKey = EVENT_EFFECTS_CHANGED;
	OnEffectsChangedEvent(int turn, Card* card) : Event(turn) {
		type = EVENT_EFFECTS_CHANGED;
		this->card = card;
	}
	Card* card;
};

//Triggers whenever a player's turn ends
struct OnTurnEndEvent : Event {
	static constexpr eEventType typeKey = EVENT_TURN_END;
	OnTurnEndEvent(int turn, Player* player) : Event(turn) {
		type = EVENT_TURN_END;
		this->player = player;
	}
	Player* player;
};

//Triggers whenever a player's turn starts
struct OnTurnStartEvent : Event {
	static constexpr eEventType typeKey = EVENT_TURN_START;
	OnTurnStartEvent(int turn, Player* player) : Event(turn) {
		type = EVENT_TURN_START;
		this->player = player;
	}
	Player* player;
};
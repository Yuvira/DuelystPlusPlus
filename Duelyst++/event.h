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
	EVENT_TURN_END,
	EVENT_TURN_START,
	EVENT_ADD_THIS,
	EVENT_REMOVE_THIS,
	EVENT_PRE_CAST_THIS,
	EVENT_RESOLVE_THIS
};

//Define
class Player;
class BoardTile;
class Card;
class Unit;

//Event object
struct Event {
	Event(int turn) {
		type = EVENT_NONE;
		this->turn = turn;
	}
	virtual ~Event() = default;
	virtual Event* Clone() const = 0;
	template <typename T> T* As() { return type == T::typeKey ? static_cast<T*>(this) : nullptr; }
	template <typename T> const T* As() const { return type == T::typeKey ? static_cast<const T*>(this) : nullptr; }
	eEventType type;
	int turn;
};

//Triggers whenever a player casts a card from their action bar
struct OnCastEvent : Event {
	static constexpr eEventType typeKey = EVENT_CAST;
	Event* Clone() const override { return new OnCastEvent(*this); }
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
	Event* Clone() const override { return new OnSummonEvent(*this); }
	OnSummonEvent(int turn, Unit* unit, bool wasCast) : Event(turn) {
		type = EVENT_SUMMON;
		this->unit = unit;
		this->wasCast = wasCast;
	}
	Unit* unit;
	bool wasCast;
};

//Triggers whenever a unit dies
struct OnDeathEvent : Event {
	static constexpr eEventType typeKey = EVENT_DEATH;
	Event* Clone() const override { return new OnDeathEvent(*this); }
	OnDeathEvent(int turn, Unit* unit) : Event(turn) {
		type = EVENT_DEATH;
		this->unit = unit;
	}
	Unit* unit;
};

//Triggers whenever a unit attacks another unit
struct OnAttackEvent : Event {
	static constexpr eEventType typeKey = EVENT_ATTACK;
	Event* Clone() const override { return new OnAttackEvent(*this); }
	OnAttackEvent(int turn, Unit* source, Unit* target, bool isCounterattack) : Event(turn) {
		type = EVENT_ATTACK;
		this->source = source;
		this->target = target;
		this->isCounterattack = isCounterattack;
	}
	Unit* source;
	Unit* target;
	bool isCounterattack;
};

//Triggers whenever a unit is dealt damage
struct OnDamageEvent : Event {
	static constexpr eEventType typeKey = EVENT_DAMAGE;
	Event* Clone() const override { return new OnDamageEvent(*this); }
	OnDamageEvent(int turn, Card* source, Unit* target, int damage) : Event(turn) {
		type = EVENT_DAMAGE;
		this->source = source;
		this->target = target;
		this->damage = damage;
	}
	Card* source;
	Unit* target;
	int damage;
};

//Triggers whenever a unit is healed
struct OnHealEvent : Event {
	static constexpr eEventType typeKey = EVENT_HEAL;
	Event* Clone() const override { return new OnHealEvent(*this); }
	OnHealEvent(int turn, Card* source, Unit* target, int heal) : Event(turn) {
		type = EVENT_HEAL;
		this->source = source;
		this->target = target;
		this->heal = heal;
	}
	Card* source;
	Unit* target;
	int heal;
};

//Triggers whenever a unit moves or is moved
struct OnMoveEvent : Event {
	static constexpr eEventType typeKey = EVENT_MOVE;
	Event* Clone() const override { return new OnMoveEvent(*this); }
	OnMoveEvent(int turn, Unit* unit, bool wasTeleported) : Event(turn) {
		type = EVENT_MOVE;
		this->unit = unit;
		this->wasTeleported = wasTeleported;
	}
	Unit* unit;
	bool wasTeleported;
};

//Triggers whenever a player draws a card from their deck
struct OnDrawEvent : Event {
	static constexpr eEventType typeKey = EVENT_DRAW;
	Event* Clone() const override { return new OnDrawEvent(*this); }
	OnDrawEvent(int turn, Card* card) : Event(turn) {
		type = EVENT_DRAW;
		this->card = card;
	}
	Card* card;
};

//Triggers whenever a player has a card added to their hand from somewhere other than their deck
struct OnAddToHandEvent : Event {
	static constexpr eEventType typeKey = EVENT_ADD_TO_HAND;
	Event* Clone() const override { return new OnAddToHandEvent(*this); }
	OnAddToHandEvent(int turn, Card* card) : Event(turn) {
		type = EVENT_ADD_TO_HAND;
		this->card = card;
	}
	Card* card;
};

//Triggers whenever a player replaces a card in their hand
struct OnReplaceEvent : Event {
	static constexpr eEventType typeKey = EVENT_REPLACE;
	Event* Clone() const override { return new OnReplaceEvent(*this); }
	OnReplaceEvent(int turn, Card* card, bool sendToDeck) : Event(turn) {
		type = EVENT_REPLACE;
		this->card = card;
		this->sendToDeck = sendToDeck;
	}
	Card* card;
	bool sendToDeck;
};

//Triggers whenever a player's turn ends
struct OnTurnEndEvent : Event {
	static constexpr eEventType typeKey = EVENT_TURN_END;
	Event* Clone() const override { return new OnTurnEndEvent(*this); }
	OnTurnEndEvent(int turn, Player* player) : Event(turn) {
		type = EVENT_TURN_END;
		this->player = player;
	}
	Player* player;
};

//Triggers whenever a player's turn starts
struct OnTurnStartEvent : Event {
	static constexpr eEventType typeKey = EVENT_TURN_START;
	Event* Clone() const override { return new OnTurnStartEvent(*this); }
	OnTurnStartEvent(int turn, Player* player) : Event(turn) {
		type = EVENT_TURN_START;
		this->player = player;
	}
	Player* player;
};

//Triggers on an effect when it's added to a card or enters the field
struct OnAddThisEvent : Event {
	static constexpr eEventType typeKey = EVENT_ADD_THIS;
	Event* Clone() const override { return new OnAddThisEvent(*this); }
	OnAddThisEvent() : Event(-1) {
		type = EVENT_ADD_THIS;
	}
};

//Triggers on an effect when it's removed from a card or leaves the field
struct OnRemoveThisEvent : Event {
	static constexpr eEventType typeKey = EVENT_REMOVE_THIS;
	Event* Clone() const override { return new OnRemoveThisEvent(*this); }
	OnRemoveThisEvent() : Event(-1) {
		type = EVENT_REMOVE_THIS;
	}
};

//Triggers on an effect just before it's cast (Opening Gambit)
struct OnPreCastThisEvent : Event {
	static constexpr eEventType typeKey = EVENT_PRE_CAST_THIS;
	Event* Clone() const override { return new OnPreCastThisEvent(*this); }
	OnPreCastThisEvent(BoardTile* tile) : Event(-1) {
		type = EVENT_PRE_CAST_THIS;
		this->tile = tile;
	}
	BoardTile* tile;
};

//Triggers on an effect as the card it's attached to resolves (spell effects)
struct OnResolveThisEvent : Event {
	static constexpr eEventType typeKey = EVENT_RESOLVE_THIS;
	Event* Clone() const override { return new OnResolveThisEvent(*this); }
	OnResolveThisEvent(BoardTile* tile) : Event(-1) {
		type = EVENT_RESOLVE_THIS;
		this->tile = tile;
	}
	BoardTile* tile;
};
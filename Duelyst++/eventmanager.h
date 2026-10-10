#pragma once

//Include
#include "player.h"

//Define
class Game;

//Event type enum
enum eEventType {
	EVENT_NONE,
	EVENT_CAST,
	EVENT_SUMMON,
	EVENT_DEATH,
	EVENT_ATTACK,
	EVENT_DAMAGE_DEALT,
	EVENT_HEALED,
	EVENT_MOVE,
	EVENT_DRAW,
	EVENT_REPLACE,
	EVENT_TURN_END,
	EVENT_TURN_START
};

//Event object
struct Event {
	Event(eEventType type, int turn, Player* player, Card* source, Card* target, BoardTile* tile, int value, bool flag);
	eEventType type = EVENT_NONE;
	int turn = 0;
	Player* player = nullptr;
	Card* source = nullptr;
	Card* target = nullptr;
	BoardTile* tile = nullptr;
	int value = 0;
	bool flag = false;
};

//Game class
class EventManager {
public:
	EventManager();
	~EventManager();
	void SendOnCast(Card* card, BoardTile* tile);
	void SendOnSummon(Minion* minion, bool actionBar);
	void SendOnDeath(Minion* minion);
	void SendOnAttack(Minion* source, Minion* target, bool counter);
	void SendOnWouldDealDamage(Card* source, Minion* target, int& damage);
	void SendOnDamageDealt(Card* source, Minion* target, int damage);
	void SendOnWouldHeal(Card* source, Minion* target, int& heal);
	void SendOnHealed(Card* source, Minion* target, int heal);
	void SendOnMove(Minion* minion, bool byEffect);
	void SendOnDraw(Card* card, bool fromDeck);
	void SendOnReplace(Card* card, bool& sendToDeck);
	void SendOnEffectsChanged(Card*);
	void SendOnTurnEnd(Player* player);
	void SendOnTurnStart(Player* player);
	std::vector<Event*> GetMostRecentTurnEvents(Player* player);
	std::vector<Event> eventLog;
	Game* game;
};
#pragma once

//Include
#include "player.h"

//Define
class Game;

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
	Game* game;
};
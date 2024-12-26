//Defines
#ifndef __EVENTMANAGER_H__
#define __EVENTMANAGER_H__

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
	void SendOnDamage(Minion* source, Minion* target, int damage);
	void SendOnHeal(Minion* source, Minion* target, int heal);
	void SendOnMove(Minion* minion, bool byEffect);
	void SendOnSpellCast(Spell* spell);
	void SendOnDraw(Card* card, bool fromDeck);
	void SendOnReplace(Card* card);
	void SendOnTurnEnd(Player* player);
	void SendOnTurnStart(Player* player);
	Game* game;
};

#endif
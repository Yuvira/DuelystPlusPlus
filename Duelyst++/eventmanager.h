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
	void sendOnSummon(Unit* u, bool actionBar);
	void sendOnDeath(Unit* u);
	void sendOnAttack(Unit* u1, Unit* u2, bool counter);
	void sendOnDamage(Unit* u1, Unit* u2, int damage);
	void sendOnHeal(Unit* u1, Unit* u2, int heal);
	void sendOnMove(Unit* u, bool byEffect);
	void sendOnDraw(Card* c, bool fromDeck);
	void sendOnReplace(Card* c);
	void sendOnTurnEnd(Player* p);
	void sendOnTurnStart(Player* p);
	Game* game;
};

#endif
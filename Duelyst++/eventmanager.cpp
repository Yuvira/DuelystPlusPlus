//Include
#include "game.h"

EventManager::EventManager() {}
EventManager::~EventManager() {}

//Send onSummon events
void EventManager::sendOnSummon(Unit* u, bool actionBar) {
	for (int a = 0; a < game->unit.size(); ++a) { game->unit[a]->onSummon(u, actionBar); }
	for (int a = 0; a < game->player[0].hand.size(); ++a) { game->player[0].hand[a]->onSummon(u, actionBar); }
	for (int a = 0; a < game->player[0].deck.size(); ++a) { game->player[0].deck[a]->onSummon(u, actionBar); }
	for (int a = 0; a < game->player[1].hand.size(); ++a) { game->player[1].hand[a]->onSummon(u, actionBar); }
	for (int a = 0; a < game->player[1].deck.size(); ++a) { game->player[1].deck[a]->onSummon(u, actionBar); }
}

//Send onDeath events
void EventManager::sendOnDeath(Unit* u) {
	for (int a = 0; a < game->unit.size(); ++a) { game->unit[a]->onDeath(u); }
	for (int a = 0; a < game->player[0].hand.size(); ++a) { game->player[0].hand[a]->onDeath(u); }
	for (int a = 0; a < game->player[0].deck.size(); ++a) { game->player[0].deck[a]->onDeath(u); }
	for (int a = 0; a < game->player[1].hand.size(); ++a) { game->player[1].hand[a]->onDeath(u); }
	for (int a = 0; a < game->player[1].deck.size(); ++a) { game->player[1].deck[a]->onDeath(u); }
}

//Send onAttack events
void EventManager::sendOnAttack(Unit* u1, Unit* u2) {
	for (int a = 0; a < game->unit.size(); ++a) { game->unit[a]->onAttack(u1, u2); }
	for (int a = 0; a < game->player[0].hand.size(); ++a) { game->player[0].hand[a]->onAttack(u1, u2); }
	for (int a = 0; a < game->player[0].deck.size(); ++a) { game->player[0].deck[a]->onAttack(u1, u2); }
	for (int a = 0; a < game->player[1].hand.size(); ++a) { game->player[1].hand[a]->onAttack(u1, u2); }
	for (int a = 0; a < game->player[1].deck.size(); ++a) { game->player[1].deck[a]->onAttack(u1, u2); }
}

//Send onDamage events
void EventManager::sendOnDamage(Unit* u1, Unit* u2) {
	for (int a = 0; a < game->unit.size(); ++a) { game->unit[a]->onDamage(u1, u2); }
	for (int a = 0; a < game->player[0].hand.size(); ++a) { game->player[0].hand[a]->onDamage(u1, u2); }
	for (int a = 0; a < game->player[0].deck.size(); ++a) { game->player[0].deck[a]->onDamage(u1, u2); }
	for (int a = 0; a < game->player[1].hand.size(); ++a) { game->player[1].hand[a]->onDamage(u1, u2); }
	for (int a = 0; a < game->player[1].deck.size(); ++a) { game->player[1].deck[a]->onDamage(u1, u2); }
}

//Send onTurnEnd events
void EventManager::sendOnTurnEnd(Player* p) {
	for (int a = 0; a < game->unit.size(); ++a) { game->unit[a]->onTurnEnd(p); }
	for (int a = 0; a < game->player[0].hand.size(); ++a) { game->player[0].hand[a]->onTurnEnd(p); }
	for (int a = 0; a < game->player[0].deck.size(); ++a) { game->player[0].deck[a]->onTurnEnd(p); }
	for (int a = 0; a < game->player[1].hand.size(); ++a) { game->player[1].hand[a]->onTurnEnd(p); }
	for (int a = 0; a < game->player[1].deck.size(); ++a) { game->player[1].deck[a]->onTurnEnd(p); }
}

//Send onTurnStart events
void EventManager::sendOnTurnStart(Player* p) {
	for (int a = 0; a < game->unit.size(); ++a) { game->unit[a]->onTurnStart(p); }
	for (int a = 0; a < game->player[0].hand.size(); ++a) { game->player[0].hand[a]->onTurnStart(p); }
	for (int a = 0; a < game->player[0].deck.size(); ++a) { game->player[0].deck[a]->onTurnStart(p); }
	for (int a = 0; a < game->player[1].hand.size(); ++a) { game->player[1].hand[a]->onTurnStart(p); }
	for (int a = 0; a < game->player[1].deck.size(); ++a) { game->player[1].deck[a]->onTurnStart(p); }
}
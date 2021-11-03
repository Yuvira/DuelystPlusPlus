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
void EventManager::sendOnAttack(Unit* u1, Unit* u2, bool counter) {
	for (int a = 0; a < game->unit.size(); ++a) { game->unit[a]->onAttack(u1, u2, counter); }
	for (int a = 0; a < game->player[0].hand.size(); ++a) { game->player[0].hand[a]->onAttack(u1, u2, counter); }
	for (int a = 0; a < game->player[0].deck.size(); ++a) { game->player[0].deck[a]->onAttack(u1, u2, counter); }
	for (int a = 0; a < game->player[1].hand.size(); ++a) { game->player[1].hand[a]->onAttack(u1, u2, counter); }
	for (int a = 0; a < game->player[1].deck.size(); ++a) { game->player[1].deck[a]->onAttack(u1, u2, counter); }
}

//Send onDamage events
void EventManager::sendOnDamage(Unit* u1, Unit* u2, int damage) {
	for (int a = 0; a < game->unit.size(); ++a) { game->unit[a]->onDamage(u1, u2, damage); }
	for (int a = 0; a < game->player[0].hand.size(); ++a) { game->player[0].hand[a]->onDamage(u1, u2, damage); }
	for (int a = 0; a < game->player[0].deck.size(); ++a) { game->player[0].deck[a]->onDamage(u1, u2, damage); }
	for (int a = 0; a < game->player[1].hand.size(); ++a) { game->player[1].hand[a]->onDamage(u1, u2, damage); }
	for (int a = 0; a < game->player[1].deck.size(); ++a) { game->player[1].deck[a]->onDamage(u1, u2, damage); }
}

//Send onHeal events
void EventManager::sendOnHeal(Unit* u1, Unit* u2, int heal) {
	for (int a = 0; a < game->unit.size(); ++a) { game->unit[a]->onHeal(u1, u2, heal); }
	for (int a = 0; a < game->player[0].hand.size(); ++a) { game->player[0].hand[a]->onHeal(u1, u2, heal); }
	for (int a = 0; a < game->player[0].deck.size(); ++a) { game->player[0].deck[a]->onHeal(u1, u2, heal); }
	for (int a = 0; a < game->player[1].hand.size(); ++a) { game->player[1].hand[a]->onHeal(u1, u2, heal); }
	for (int a = 0; a < game->player[1].deck.size(); ++a) { game->player[1].deck[a]->onHeal(u1, u2, heal); }
}

//Send onMoved events
void EventManager::sendOnMove(Unit* u, bool byEffect) {
	for (int a = 0; a < game->unit.size(); ++a) { game->unit[a]->onMove(u, byEffect); }
	for (int a = 0; a < game->player[0].hand.size(); ++a) { game->player[0].hand[a]->onMove(u, byEffect); }
	for (int a = 0; a < game->player[0].deck.size(); ++a) { game->player[0].deck[a]->onMove(u, byEffect); }
	for (int a = 0; a < game->player[1].hand.size(); ++a) { game->player[1].hand[a]->onMove(u, byEffect); }
	for (int a = 0; a < game->player[1].deck.size(); ++a) { game->player[1].deck[a]->onMove(u, byEffect); }
}

//Send onSpellCast events
void EventManager::sendOnSpellCast(Spell* s) {
	for (int a = 0; a < game->unit.size(); ++a) { game->unit[a]->onSpellCast(s); }
	for (int a = 0; a < game->player[0].hand.size(); ++a) { game->player[0].hand[a]->onSpellCast(s); }
	for (int a = 0; a < game->player[0].deck.size(); ++a) { game->player[0].deck[a]->onSpellCast(s); }
	for (int a = 0; a < game->player[1].hand.size(); ++a) { game->player[1].hand[a]->onSpellCast(s); }
	for (int a = 0; a < game->player[1].deck.size(); ++a) { game->player[1].deck[a]->onSpellCast(s); }
}

//Send onDraw events
void EventManager::sendOnDraw(Card* c, bool fromDeck) {
	for (int a = 0; a < game->unit.size(); ++a) { game->unit[a]->onDraw(c, fromDeck); }
	for (int a = 0; a < game->player[0].hand.size(); ++a) { game->player[0].hand[a]->onDraw(c, fromDeck); }
	for (int a = 0; a < game->player[0].deck.size(); ++a) { game->player[0].deck[a]->onDraw(c, fromDeck); }
	for (int a = 0; a < game->player[1].hand.size(); ++a) { game->player[1].hand[a]->onDraw(c, fromDeck); }
	for (int a = 0; a < game->player[1].deck.size(); ++a) { game->player[1].deck[a]->onDraw(c, fromDeck); }
}

//Send onReplace events
void EventManager::sendOnReplace(Card* c) {
	for (int a = 0; a < game->unit.size(); ++a) { game->unit[a]->onReplace(c); }
	for (int a = 0; a < game->player[0].hand.size(); ++a) { game->player[0].hand[a]->onReplace(c); }
	for (int a = 0; a < game->player[0].deck.size(); ++a) { game->player[0].deck[a]->onReplace(c); }
	for (int a = 0; a < game->player[1].hand.size(); ++a) { game->player[1].hand[a]->onReplace(c); }
	for (int a = 0; a < game->player[1].deck.size(); ++a) { game->player[1].deck[a]->onReplace(c); }
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
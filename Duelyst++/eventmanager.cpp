//Include
#include "game.h"

#pragma region Constructor

//Event manager constructor
EventManager::EventManager() {
	game = nullptr;
}
EventManager::~EventManager() {}

#pragma endregion

#pragma region Events

//Send onSpellCast events
void EventManager::SendOnCast(Card* card, BoardTile* tile) {
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnCast(card, tile); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnCast(card, tile); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnCast(card, tile); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnCast(card, tile); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnCast(card, tile); }
}

//Send onSummon events
void EventManager::SendOnSummon(Minion* minion, bool actionBar) {
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnSummon(minion, actionBar); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnSummon(minion, actionBar); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnSummon(minion, actionBar); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnSummon(minion, actionBar); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnSummon(minion, actionBar); }
}

//Send onDeath events
void EventManager::SendOnDeath(Minion* minion) {
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnDeath(minion); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnDeath(minion); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnDeath(minion); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnDeath(minion); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnDeath(minion); }
}

//Send onAttack events
void EventManager::SendOnAttack(Minion* source, Minion* target, bool counter) {
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnAttack(source, target, counter); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnAttack(source, target, counter); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnAttack(source, target, counter); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnAttack(source, target, counter); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnAttack(source, target, counter); }
}

//Send onDamage events
void EventManager::SendOnDamage(Minion* source, Minion* target, int damage) {
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnDamage(source, target, damage); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnDamage(source, target, damage); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnDamage(source, target, damage); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnDamage(source, target, damage); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnDamage(source, target, damage); }
}

//Send onHeal events
void EventManager::SendOnHeal(Minion* source, Minion* target, int heal) {
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnHeal(source, target, heal); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnHeal(source, target, heal); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnHeal(source, target, heal); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnHeal(source, target, heal); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnHeal(source, target, heal); }
}

//Send onMoved events
void EventManager::SendOnMove(Minion* minion, bool byEffect) {
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnMove(minion, byEffect); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnMove(minion, byEffect); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnMove(minion, byEffect); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnMove(minion, byEffect); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnMove(minion, byEffect); }
}

//Send onDraw events
void EventManager::SendOnDraw(Card* card, bool fromDeck) {
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnDraw(card, fromDeck); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnDraw(card, fromDeck); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnDraw(card, fromDeck); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnDraw(card, fromDeck); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnDraw(card, fromDeck); }
}

//Send onReplace events
void EventManager::SendOnReplace(Card* card) {
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnReplace(card); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnReplace(card); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnReplace(card); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnReplace(card); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnReplace(card); }
}

//Send onTurnEnd events
void EventManager::SendOnTurnEnd(Player* player) {
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnTurnEnd(player); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnTurnEnd(player); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnTurnEnd(player); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnTurnEnd(player); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnTurnEnd(player); }
}

//Send onTurnStart events
void EventManager::SendOnTurnStart(Player* player) {
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnTurnStart(player); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnTurnStart(player); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnTurnStart(player); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnTurnStart(player); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnTurnStart(player); }
}

#pragma endregion
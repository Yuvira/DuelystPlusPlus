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
	game->Log(game->GetCardString(card) + " was cast");
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnCast(card, tile); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnCast(card, tile); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnCast(card, tile); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnCast(card, tile); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnCast(card, tile); }
}

//Send onSummon events
void EventManager::SendOnSummon(Minion* minion, bool actionBar) {
	if (actionBar)
		game->Log(game->GetCardString(minion) + " was summoned");
	else
		game->Log(game->GetCardString(minion) + " was created");
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnSummon(minion, actionBar); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnSummon(minion, actionBar); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnSummon(minion, actionBar); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnSummon(minion, actionBar); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnSummon(minion, actionBar); }
}

//Send onDeath events
void EventManager::SendOnDeath(Minion* minion) {
	game->Log(game->GetCardString(minion) + " died");
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnDeath(minion); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnDeath(minion); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnDeath(minion); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnDeath(minion); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnDeath(minion); }
}

//Send onAttack events
void EventManager::SendOnAttack(Minion* source, Minion* target, bool counter) {
	if (counter)
		game->Log(game->GetCardString(source) + " counterattacked " + game->GetCardString(target));
	else
		game->Log(game->GetCardString(source) + " attacked " + game->GetCardString(target));
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnAttack(source, target, counter); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnAttack(source, target, counter); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnAttack(source, target, counter); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnAttack(source, target, counter); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnAttack(source, target, counter); }
}

//Send onWouldDealDamage events
void EventManager::SendOnWouldDealDamage(Card* source, Minion* target, int& damage) {
	game->Log(game->GetCardString(target) + " will be damaged for " + std::to_string(damage) + " points");
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnWouldDealDamage(source, target, damage); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnWouldDealDamage(source, target, damage); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnWouldDealDamage(source, target, damage); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnWouldDealDamage(source, target, damage); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnWouldDealDamage(source, target, damage); }
}

//Send onDamageDealt events
void EventManager::SendOnDamageDealt(Card* source, Minion* target, int damage) {
	game->Log(game->GetCardString(target) + " was damaged for " + std::to_string(damage) + " points");
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnDamageDealt(source, target, damage); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnDamageDealt(source, target, damage); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnDamageDealt(source, target, damage); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnDamageDealt(source, target, damage); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnDamageDealt(source, target, damage); }
}

//Send onHealed events
void EventManager::SendOnWouldHeal(Card* source, Minion* target, int& heal) {
	game->Log(game->GetCardString(target) + " will be healed for " + std::to_string(heal) + " points");
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnWouldHeal(source, target, heal); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnWouldHeal(source, target, heal); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnWouldHeal(source, target, heal); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnWouldHeal(source, target, heal); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnWouldHeal(source, target, heal); }
}

//Send onHealed events
void EventManager::SendOnHealed(Card* source, Minion* target, int heal) {
	game->Log(game->GetCardString(target) + " was healed for " + std::to_string(heal) + " points");
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnHealed(source, target, heal); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnHealed(source, target, heal); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnHealed(source, target, heal); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnHealed(source, target, heal); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnHealed(source, target, heal); }
}

//Send onMoved events
void EventManager::SendOnMove(Minion* minion, bool byEffect) {
	if (byEffect)
		game->Log(game->GetCardString(minion) + " was moved");
	else
		game->Log(game->GetCardString(minion) + " moved");
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnMove(minion, byEffect); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnMove(minion, byEffect); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnMove(minion, byEffect); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnMove(minion, byEffect); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnMove(minion, byEffect); }
}

//Send onDraw events
void EventManager::SendOnDraw(Card* card, bool fromDeck) {
	if (fromDeck)
		game->Log("Drew " + game->GetCardString(card) + " from deck");
	else
		game->Log("Added " + game->GetCardString(card) + " to hand");
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnDraw(card, fromDeck); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnDraw(card, fromDeck); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnDraw(card, fromDeck); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnDraw(card, fromDeck); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnDraw(card, fromDeck); }
}

//Send onReplace events
void EventManager::SendOnReplace(Card* card, bool& sendToDeck) {
	game->Log("Replaced " + game->GetCardString(card));
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnReplace(card, sendToDeck); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnReplace(card, sendToDeck); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnReplace(card, sendToDeck); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnReplace(card, sendToDeck); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnReplace(card, sendToDeck); }
}

//Send onEffectsChanged events
void EventManager::SendOnEffectsChanged(Card* card) {
	game->Log("Effects changed on " + game->GetCardString(card));
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnEffectsChanged(card); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnEffectsChanged(card); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnEffectsChanged(card); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnEffectsChanged(card); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnEffectsChanged(card); }
}

//Send onTurnEnd events
void EventManager::SendOnTurnEnd(Player* player) {
	game->Log("Ending {Turn " + std::to_string(game->turnCount) + "} for " + game->GetPlayerString(player));
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnTurnEnd(player); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnTurnEnd(player); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnTurnEnd(player); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnTurnEnd(player); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnTurnEnd(player); }
}

//Send onTurnStart events
void EventManager::SendOnTurnStart(Player* player) {
	game->Log("Starting {Turn " + std::to_string(game->turnCount) + "} for " + game->GetPlayerString(player));
	for (int i = 0; i < game->minions.size(); ++i) { game->minions[i]->OnTurnStart(player); }
	for (int i = 0; i < game->players[0].hand.size(); ++i) { game->players[0].hand[i]->OnTurnStart(player); }
	for (int i = 0; i < game->players[0].deck.size(); ++i) { game->players[0].deck[i]->OnTurnStart(player); }
	for (int i = 0; i < game->players[1].hand.size(); ++i) { game->players[1].hand[i]->OnTurnStart(player); }
	for (int i = 0; i < game->players[1].deck.size(); ++i) { game->players[1].deck[i]->OnTurnStart(player); }
}

#pragma endregion
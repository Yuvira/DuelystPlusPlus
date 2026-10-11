//Include
#include "game.h"

#pragma region Constructor

//Event object constructor
Event::Event(eEventType type, int turn, Player* player, Card* source, Card* target, BoardTile* tile, int value, bool flag) {
	this->type = type;
	this->turn = turn;
	this->player = player;
	this->source = source;
	this->target = target;
	this->tile = tile;
	this->value = value;
	this->flag = flag;
}

//Event manager constructor
EventManager::EventManager() { game = nullptr; }
EventManager::~EventManager() {}

#pragma endregion

#pragma region Events

//Send onSpellCast events
void EventManager::SendOnCast(Card* card, BoardTile* tile) {
	game->Log(game->GetCardString(card) + " was cast");
	eventLog.push_back(Event(EVENT_CAST, game->GetTurnCount(), card->owner, card, nullptr, tile, 0, false));
	for (Minion* m : game->GetMinions()) { m->OnCast(card, tile); }
	for (Card* c : game->players[0].hand) { c->OnCast(card, tile); }
	for (Card* c : game->players[0].deck) { c->OnCast(card, tile); }
	for (Card* c : game->players[1].hand) { c->OnCast(card, tile); }
	for (Card* c : game->players[1].deck) { c->OnCast(card, tile); }
}

//Send onSummon events
void EventManager::SendOnSummon(Minion* minion, bool actionBar) {
	if (actionBar)
		game->Log(game->GetCardString(minion) + " was summoned");
	else
		game->Log(game->GetCardString(minion) + " was created");
	eventLog.push_back(Event(EVENT_SUMMON, game->GetTurnCount(), minion->owner, minion, nullptr, minion->curTile, 0, actionBar));
	for (Minion* m : game->GetMinions()) { m->OnSummon(minion, actionBar); }
	for (Card* c : game->players[0].hand) { c->OnSummon(minion, actionBar); }
	for (Card* c : game->players[0].deck) { c->OnSummon(minion, actionBar); }
	for (Card* c : game->players[1].hand) { c->OnSummon(minion, actionBar); }
	for (Card* c : game->players[1].deck) { c->OnSummon(minion, actionBar); }
}

//Send onDeath events
void EventManager::SendOnDeath(Minion* minion) {
	game->Log(game->GetCardString(minion) + " died");
	eventLog.push_back(Event(EVENT_DEATH, game->GetTurnCount(), minion->owner, minion, nullptr, minion->curTile, 0, false));
	for (Minion* m : game->GetMinions()) { m->OnDeath(minion); }
	for (Card* c : game->players[0].hand) { c->OnDeath(minion); }
	for (Card* c : game->players[0].deck) { c->OnDeath(minion); }
	for (Card* c : game->players[1].hand) { c->OnDeath(minion); }
	for (Card* c : game->players[1].deck) { c->OnDeath(minion); }
}

//Send onAttack events
void EventManager::SendOnAttack(Minion* source, Minion* target, bool counter) {
	if (counter)
		game->Log(game->GetCardString(source) + " counterattacked " + game->GetCardString(target));
	else
		game->Log(game->GetCardString(source) + " attacked " + game->GetCardString(target));
	eventLog.push_back(Event(EVENT_ATTACK, game->GetTurnCount(), source->owner, source, target, source->curTile, 0, counter));
	for (Minion* m : game->GetMinions()) { m->OnAttack(source, target, counter); }
	for (Card* c : game->players[0].hand) { c->OnAttack(source, target, counter); }
	for (Card* c : game->players[0].deck) { c->OnAttack(source, target, counter); }
	for (Card* c : game->players[1].hand) { c->OnAttack(source, target, counter); }
	for (Card* c : game->players[1].deck) { c->OnAttack(source, target, counter); }
}

//Send onWouldDealDamage events
void EventManager::SendOnWouldDealDamage(Card* source, Minion* target, int& damage) {
	game->Log(game->GetCardString(target) + " will be damaged for " + std::to_string(damage) + " points");
	for (Minion* m : game->GetMinions()) { m->OnWouldDealDamage(source, target, damage); }
	for (Card* c : game->players[0].hand) { c->OnWouldDealDamage(source, target, damage); }
	for (Card* c : game->players[0].deck) { c->OnWouldDealDamage(source, target, damage); }
	for (Card* c : game->players[1].hand) { c->OnWouldDealDamage(source, target, damage); }
	for (Card* c : game->players[1].deck) { c->OnWouldDealDamage(source, target, damage); }
}

//Send onDamageDealt events
void EventManager::SendOnDamageDealt(Card* source, Minion* target, int damage) {
	game->Log(game->GetCardString(target) + " was damaged for " + std::to_string(damage) + " points");
	eventLog.push_back(Event(EVENT_DAMAGE_DEALT, game->GetTurnCount(), target->owner, source, target, target->curTile, damage, false));
	for (Minion* m : game->GetMinions()) { m->OnDamageDealt(source, target, damage); }
	for (Card* c : game->players[0].hand) { c->OnDamageDealt(source, target, damage); }
	for (Card* c : game->players[0].deck) { c->OnDamageDealt(source, target, damage); }
	for (Card* c : game->players[1].hand) { c->OnDamageDealt(source, target, damage); }
	for (Card* c : game->players[1].deck) { c->OnDamageDealt(source, target, damage); }
}

//Send onHealed events
void EventManager::SendOnWouldHeal(Card* source, Minion* target, int& heal) {
	game->Log(game->GetCardString(target) + " will be healed for " + std::to_string(heal) + " points");
	for (Minion* m : game->GetMinions()) { m->OnWouldHeal(source, target, heal); }
	for (Card* c : game->players[0].hand) { c->OnWouldHeal(source, target, heal); }
	for (Card* c : game->players[0].deck) { c->OnWouldHeal(source, target, heal); }
	for (Card* c : game->players[1].hand) { c->OnWouldHeal(source, target, heal); }
	for (Card* c : game->players[1].deck) { c->OnWouldHeal(source, target, heal); }
}

//Send onHealed events
void EventManager::SendOnHealed(Card* source, Minion* target, int heal) {
	game->Log(game->GetCardString(target) + " was healed for " + std::to_string(heal) + " points");
	eventLog.push_back(Event(EVENT_HEALED, game->GetTurnCount(), target->owner, source, target, target->curTile, heal, false));
	for (Minion* m : game->GetMinions()) { m->OnHealed(source, target, heal); }
	for (Card* c : game->players[0].hand) { c->OnHealed(source, target, heal); }
	for (Card* c : game->players[0].deck) { c->OnHealed(source, target, heal); }
	for (Card* c : game->players[1].hand) { c->OnHealed(source, target, heal); }
	for (Card* c : game->players[1].deck) { c->OnHealed(source, target, heal); }
}

//Send onMoved events
void EventManager::SendOnMove(Minion* minion, bool byEffect) {
	if (byEffect)
		game->Log(game->GetCardString(minion) + " was moved");
	else
		game->Log(game->GetCardString(minion) + " moved");
	eventLog.push_back(Event(EVENT_MOVE, game->GetTurnCount(), minion->owner, minion, nullptr, minion->curTile, 0, byEffect));
	for (Minion* m : game->GetMinions()) { m->OnMove(minion, byEffect); }
	for (Card* c : game->players[0].hand) { c->OnMove(minion, byEffect); }
	for (Card* c : game->players[0].deck) { c->OnMove(minion, byEffect); }
	for (Card* c : game->players[1].hand) { c->OnMove(minion, byEffect); }
	for (Card* c : game->players[1].deck) { c->OnMove(minion, byEffect); }
}

//Send onDraw events
void EventManager::SendOnDraw(Card* card, bool fromDeck) {
	if (fromDeck)
		game->Log("Drew " + game->GetCardString(card) + " from deck");
	else
		game->Log("Added " + game->GetCardString(card) + " to hand");
	eventLog.push_back(Event(EVENT_DRAW, game->GetTurnCount(), card->owner, card, nullptr, nullptr, 0, fromDeck));
	for (Minion* m : game->GetMinions()) { m->OnDraw(card, fromDeck); }
	for (Card* c : game->players[0].hand) { c->OnDraw(card, fromDeck); }
	for (Card* c : game->players[0].deck) { c->OnDraw(card, fromDeck); }
	for (Card* c : game->players[1].hand) { c->OnDraw(card, fromDeck); }
	for (Card* c : game->players[1].deck) { c->OnDraw(card, fromDeck); }
}

//Send onReplace events
void EventManager::SendOnReplace(Card* card, bool& sendToDeck) {
	game->Log("Replaced " + game->GetCardString(card));
	eventLog.push_back(Event(EVENT_REPLACE, game->GetTurnCount(), card->owner, card, nullptr, nullptr, 0, false));
	for (Minion* m : game->GetMinions()) { m->OnReplace(card, sendToDeck); }
	for (Card* c : game->players[0].hand) { c->OnReplace(card, sendToDeck); }
	for (Card* c : game->players[0].deck) { c->OnReplace(card, sendToDeck); }
	for (Card* c : game->players[1].hand) { c->OnReplace(card, sendToDeck); }
	for (Card* c : game->players[1].deck) { c->OnReplace(card, sendToDeck); }
}

//Send onEffectsChanged events
void EventManager::SendOnEffectsChanged(Card* card) {
	game->Log("Effects changed on " + game->GetCardString(card));
	for (Minion* m : game->GetMinions()) { m->OnEffectsChanged(card); }
	for (Card* c : game->players[0].hand) { c->OnEffectsChanged(card); }
	for (Card* c : game->players[0].deck) { c->OnEffectsChanged(card); }
	for (Card* c : game->players[1].hand) { c->OnEffectsChanged(card); }
	for (Card* c : game->players[1].deck) { c->OnEffectsChanged(card); }
}

//Send onTurnEnd events
void EventManager::SendOnTurnEnd(Player* player) {
	game->Log("Ending {Turn " + std::to_string(game->GetTurnCount()) + "} for " + game->GetPlayerString(player));
	eventLog.push_back(Event(EVENT_TURN_END, game->GetTurnCount(), player, nullptr, nullptr, nullptr, 0, false));
	for (Minion* m : game->GetMinions()) { m->OnTurnEnd(player); }
	for (Card* c : game->players[0].hand) { c->OnTurnEnd(player); }
	for (Card* c : game->players[0].deck) { c->OnTurnEnd(player); }
	for (Card* c : game->players[1].hand) { c->OnTurnEnd(player); }
	for (Card* c : game->players[1].deck) { c->OnTurnEnd(player); }
}

//Send onTurnStart events
void EventManager::SendOnTurnStart(Player* player) {
	game->Log("Starting {Turn " + std::to_string(game->GetTurnCount()) + "} for " + game->GetPlayerString(player));
	eventLog.push_back(Event(EVENT_TURN_START, game->GetTurnCount(), player, nullptr, nullptr, nullptr, 0, false));
	for (Minion* m : game->GetMinions()) { m->OnTurnStart(player); }
	for (Card* c : game->players[0].hand) { c->OnTurnStart(player); }
	for (Card* c : game->players[0].deck) { c->OnTurnStart(player); }
	for (Card* c : game->players[1].hand) { c->OnTurnStart(player); }
	for (Card* c : game->players[1].deck) { c->OnTurnStart(player); }
}

#pragma endregion

#pragma region Utils

//Get all events from a player's most recent turn (including the current one)
std::vector<Event*> EventManager::GetMostRecentTurnEvents(Player* player) {
	int max = -1;
	std::vector<Event*> valid;
	for (Event& event : eventLog)
		if (event.type == EVENT_TURN_START && event.player == player && event.turn > max)
			max = event.turn;
	for (Event& event : eventLog)
		if (event.turn == max)
			valid.push_back(&event);
	return valid;
}

#pragma endregion
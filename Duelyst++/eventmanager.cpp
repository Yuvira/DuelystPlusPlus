//Include
#include "eventmanager.h"
#include "event.h"
#include "effect.h"
#include "game.h"
#include "card.h"
#include "player.h"

#pragma region Constructor

//Event manager constructor
EventManager::EventManager() {}
EventManager::~EventManager() {}

#pragma endregion

#pragma region Events

//Send pre-events (for modifying event properties before resolution)
void EventManager::NotifyPreEvent(Game* game, Event* event) {

	//Turn player
	for (Unit* u : game->GetUnits()) {
		if (u->owner == game->GetTurnPlayer()) {
			for (Effect* e : u->GetEffects()) {
				e->TriggerPreEvent(EffectContext(e, u, game), event);
	}}}
	for (Artifact* a : game->GetTurnPlayer()->GetArtifacts())
		for (Effect* e : a->GetEffects())
			e->TriggerPreEvent(EffectContext(e, a, game), event);
	for (Card* c : game->GetTurnPlayer()->GetDeckAndHand())
		for (Effect* e : c->GetEffects())
			e->TriggerPreEvent(EffectContext(e, c, game), event);

	//Non-turn player
	for (Unit* u : game->GetUnits()) {
		if (u->owner == game->GetNonTurnPlayer()) {
			for (Effect* e : u->GetEffects()) {
				e->TriggerPreEvent(EffectContext(e, u, game), event);
	}}}
	for (Artifact* a : game->GetNonTurnPlayer()->GetArtifacts())
		for (Effect* e : a->GetEffects())
			e->TriggerPreEvent(EffectContext(e, a, game), event);
	for (Card* c : game->GetNonTurnPlayer()->GetDeckAndHand())
		for (Effect* e : c->GetEffects())
			e->TriggerPreEvent(EffectContext(e, c, game), event);

	//Auras
	for (Aura* a : game->GetAuras())
		a->TriggerPreEvent(EffectContext(a, a->GetSourceCard(), game), event);

	//Tiles
	for (Tile* t : game->GetTiles())
		for (Effect* e : t->GetEffects())
			e->TriggerPreEvent(EffectContext( e, t, game), event);

	//Delayed triggers
	for (Spell* s : game->GetDelayedTriggers())
		if (s->GetSpellEffect() != nullptr)
			s->GetSpellEffect()->TriggerPreEvent(EffectContext(s->GetSpellEffect(), s, game), event);

}

//Send post-events (for triggers in response to resolution) and log history
void EventManager::TriggerPostEvent(Game* game, Event* event) {

	//Log to console
	LogEvent(game, event);

	//Turn player
	for (Unit* u : game->GetUnits()) {
		if (u->owner == game->GetTurnPlayer()) {
			for (Effect* e : u->GetEffects()) {
				e->TriggerPostEvent(EffectContext(e, u, game), event);
	}}}
	for (Artifact* a : game->GetTurnPlayer()->GetArtifacts())
		for (Effect* e : a->GetEffects())
			e->TriggerPostEvent(EffectContext(e, a, game), event);
	for (Card* c : game->GetTurnPlayer()->GetDeckAndHand())
		for (Effect* e : c->GetEffects())
			e->TriggerPostEvent(EffectContext(e, c, game), event);

	//Non-turn player
	for (Unit* u : game->GetUnits()) {
		if (u->owner == game->GetNonTurnPlayer()) {
			for (Effect* e : u->GetEffects()) {
				e->TriggerPostEvent(EffectContext(e, u, game), event);
	}}}
	for (Artifact* a : game->GetNonTurnPlayer()->GetArtifacts())
		for (Effect* e : a->GetEffects())
			e->TriggerPostEvent(EffectContext(e, a, game), event);
	for (Card* c : game->GetNonTurnPlayer()->GetDeckAndHand())
		for (Effect* e : c->GetEffects())
			e->TriggerPostEvent(EffectContext(e, c, game), event);

	//Auras
	for (Aura* a : game->GetAuras())
		a->TriggerPostEvent(EffectContext(a, a->GetSourceCard(), game), event);

	//Tiles
	for (Tile* t : game->GetTiles())
		for (Effect* e : t->GetEffects())
			e->TriggerPostEvent(EffectContext(e, t, game), event);

	//Delayed triggers
	for (Spell* s : game->GetDelayedTriggers())
		if (s->GetSpellEffect() != nullptr)
			s->GetSpellEffect()->TriggerPostEvent(EffectContext(s->GetSpellEffect(), s, game), event);

	//Push to event history
	eventLog.push_back(event->Clone());

}

#pragma endregion

#pragma region Logging

//Log event to game log window
void EventManager::LogEvent(Game* game, Event* event) {
	switch (event->type) {
	case EVENT_CAST:
	{
		OnCastEvent* castEvent = event->As<OnCastEvent>();
		game->Log(game->GetCardString(castEvent->card) + " was cast");
		break;
	}
	case EVENT_SUMMON:
	{
		OnSummonEvent* summonEvent = event->As<OnSummonEvent>();
		if (summonEvent->wasCast)
			game->Log(game->GetCardString(summonEvent->unit) + " was summoned");
		else
			game->Log(game->GetCardString(summonEvent->unit) + " was created");
		break;
	}
	case EVENT_DEATH:
	{
		OnDeathEvent* deathEvent = event->As<OnDeathEvent>();
		game->Log(game->GetCardString(deathEvent->unit) + " died");
		break;
	}
	case EVENT_ATTACK:
	{
		OnAttackEvent* attackEvent = event->As<OnAttackEvent>();
		if (attackEvent->isCounterattack)
			game->Log(game->GetCardString(attackEvent->source) + " counterattacked " + game->GetCardString(attackEvent->target));
		else
			game->Log(game->GetCardString(attackEvent->source) + " attacked " + game->GetCardString(attackEvent->target));
		break;
	}
	case EVENT_DAMAGE:
	{
		OnDamageEvent* damageEvent = event->As<OnDamageEvent>();
		game->Log(game->GetCardString(damageEvent->target) + " was damaged for " + std::to_string(damageEvent->damage) + " points");
		break;
	}
	case EVENT_HEAL:
	{
		OnHealEvent* healEvent = event->As<OnHealEvent>();
		game->Log(game->GetCardString(healEvent->target) + " was healed for " + std::to_string(healEvent->heal) + " points");
		break;
	}
	case EVENT_MOVE:
	{
		OnMoveEvent* moveEvent = event->As<OnMoveEvent>();
		if (moveEvent->wasTeleported)
			game->Log(game->GetCardString(moveEvent->unit) + " was teleported");
		else
			game->Log(game->GetCardString(moveEvent->unit) + " moved");
		break;
	}
	case EVENT_DRAW:
	{
		OnDrawEvent* drawEvent = event->As<OnDrawEvent>();
		game->Log("Drew " + game->GetCardString(drawEvent->card) + " from deck");
		break;
	}
	case EVENT_ADD_TO_HAND:
	{
		OnAddToHandEvent* addToHandEvent = event->As<OnAddToHandEvent>();
		game->Log("Added " + game->GetCardString(addToHandEvent->card) + " to hand");
		break;
	}
	case EVENT_REPLACE:
	{
		OnReplaceEvent* replaceEvent = event->As<OnReplaceEvent>();
		game->Log("Replaced " + game->GetCardString(replaceEvent->card));
		break;
	}
	case EVENT_TURN_END:
	{
		OnTurnEndEvent* turnEndEvent = event->As<OnTurnEndEvent>();
		game->Log("Ending {Turn " + std::to_string(game->GetTurnCount()) + "} for " + game->GetPlayerString(turnEndEvent->player));
		break;
	}
	case EVENT_TURN_START:
	{
		OnTurnStartEvent* turnStartEvent = event->As<OnTurnStartEvent>();
		game->Log("Starting {Turn " + std::to_string(game->GetTurnCount()) + "} for " + game->GetPlayerString(turnStartEvent->player));
		break;
	}
	default:
		break;
	}
}

#pragma endregion

#pragma region Utils

//Get all events from a player's most recent turn (including the current one)
std::vector<Event*> EventManager::GetMostRecentTurnEvents(Player* player) {
	int max = -1;
	std::vector<Event*> valid;
	OnTurnStartEvent* turnStartEvent = nullptr;
	for (Event* event : eventLog)
		if (turnStartEvent = event->As<OnTurnStartEvent>())
			if (turnStartEvent->player == player && turnStartEvent->turn > max)
				max = turnStartEvent->turn;
	for (Event* event : eventLog)
		if (event->turn == max)
			valid.push_back(event);
	return valid;
}

//Get all events since the end of a player's last turn (not including the current one
std::vector<Event*> EventManager::GetEventsSincePlayersLastTurn(Player* player) {
	int max = -1;
	std::vector<Event*> valid;
	OnTurnEndEvent* turnEndEvent = nullptr;
	for (Event* event : eventLog)
		if (turnEndEvent = event->As<OnTurnEndEvent>())
			if (turnEndEvent->player == player && turnEndEvent->turn > max)
				max = turnEndEvent->turn;
	for (Event* event : eventLog)
		if (event->turn > max)
			valid.push_back(event);
	return valid;
}

//Check if any events from a player's most recent turn match a predicate
bool EventManager::MostRecentPlayerTurnHasEvent(Player* player, bool (*Predicate)(Player* player, Event* event)) {
	int max = -1;
	OnTurnStartEvent* turnStartEvent = nullptr;
	for (Event* event : eventLog) {
		if (turnStartEvent = event->As<OnTurnStartEvent>())
			if (turnStartEvent->player == player && turnStartEvent->turn > max)
				max = turnStartEvent->turn;
	}
	for (Event* event : eventLog) {
		if (event->turn == max)
			if (Predicate && Predicate(player, event))
				return true;
	}
	return false;
}

#pragma endregion
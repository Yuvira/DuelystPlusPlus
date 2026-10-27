#pragma once

//Include
#include <vector>

//Define
class Game;
class Player;
struct Event;

//Game class
class EventManager {
public:
	EventManager();
	~EventManager();
	void NotifyPreEvent(Game* game, Event* event);
	void TriggerPostEvent(Game* game, Event* event);
	void LogEvent(Game* game, Event* event);
	std::vector<Event*> GetMostRecentTurnEvents(Player* player);
	std::vector<Event*> GetEventsSincePlayersLastTurn(Player* player);
	bool MostRecentPlayerTurnHasEvent(Player* player, bool (*Predicate)(Player* player, Event* event));
	void DeleteEvents();
private:
	std::vector<Event*> eventLog;
};
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
	bool MostRecentPlayerTurnHasEvent(Player* player, bool (*Predicate)(Player* player, Event* event));
private:
	std::vector<Event*> eventLog;
};
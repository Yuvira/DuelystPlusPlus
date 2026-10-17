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
	void SendOnPreEvent(Game* game, Event* event);
	void SendOnPostEvent(Game* game, Event* event);
	void LogEvent(Game* game, Event* event);
	std::vector<Event*> GetMostRecentTurnEvents(Player* player);
	bool MostRecentPlayerTurnHasEvent(Player* player, bool (*Predicate)(Player* player, Event* event));
private:
	std::vector<Event*> eventLog;
};
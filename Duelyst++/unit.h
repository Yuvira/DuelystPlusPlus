//Defines
#ifndef __UNIT_H__
#define __UNIT_H__

//Include
#include <vector>
#include <functional>
#include "card.h"

//Definitions
class Game;
class Player;

//Factions
enum eFaction {
	FACTION_NEUTRAL,
	FACTION_LYONAR,
	FACTION_SONGHAI,
	FACTION_VETRUVIAN,
	FACTION_ABYSSIAN,
	FACTION_MAGMAR,
	FACTION_VANAR
};

//Tribes
enum eTribe {
	TRIBE_NONE,
	TRIBE_GENERAL,
	TRIBE_ARCANYST,
	TRIBE_PET,
	TRIBE_GOLEM,
	TRIBE_MECH,
	TRIBE_DERVISH,
	TRIBE_VESPYR,
	TRIBE_STRUCTURE
};

//Unit class
class Unit : public Card {
public:
	Unit(eFaction = FACTION_NEUTRAL, eTribe = TRIBE_NONE, int = 0, int = 0, int = 0, std::string = "");
	~Unit();
	void render(Renderer& rm);
	void setPos(int x, int y, Map& m);
	void updateStatSprites();
	void doNothing();
	void doNothingU(Unit& u);
	eFaction faction;
	eTribe tribe;
	int cost;
	int attack;
	int health;
	Game* game;
	Player* player;
	Sprite sHP;
	Sprite sATK;
	std::function<void()> onSummon;
	std::function<void(Unit& u)> onSummonAny;
	std::function<void()> onDeath;
	std::function<void(Unit& u)> onDeathAny;
};

#endif
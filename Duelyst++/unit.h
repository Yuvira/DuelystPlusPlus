//Defines
#ifndef __UNIT_H__
#define __UNIT_H__

//Include
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
	Unit(eFaction = FACTION_NEUTRAL, eTribe = TRIBE_NONE, int = 0, int = 0, int = 0, std::string = "", std::string = "???");
	~Unit();
	void render(Renderer& rm);
	void attack(Unit& u);
	void setPos(int x, int y, Map& m);
	void update(bool& r);
	void updateStatSprites();
	void updateDetailStats();
	void generateDetails();
	void drawCard(Renderer& rm, int& y);
	void onSummon();
	void onSummonAny(Unit& u);
	void onDeath();
	void onDeathAny(Unit& u);
	void onAttack(Unit& u);
	void onAttacked(Unit& u);
	eFaction faction;
	eTribe tribe;
	int atk;
	int hp;
	Game* game;
	Player* player;
	Sprite sHP;
	Sprite sATK;
	std::string cardKeywords;
	std::vector<std::string> effectName;
	std::vector<std::string> effectDescription;
};

#endif
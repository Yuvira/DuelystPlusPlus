//Defines
#ifndef __CARD_H__
#define __CARD_H__

//Include
#include <algorithm>
#include "effect.h"

//Definitions
class Game;
class Player;

//Card types
enum eCard {
	CARD_UNIT,
	CARD_SPELL,
	CARD_ARTIFACT
};

//Rarities
enum eRarity {
	RARITY_NONE,
	RARITY_COMMON,
	RARITY_RARE,
	RARITY_EPIC,
	RARITY_LEGENDARY
};

//Targeting types
enum eTarget {
	TARGET_ANY,
	TARGET_UNIT,
	TARGET_ALLY,
	TARGET_ENEMY,
	TARGET_NEAR_UNIT,
	TARGET_NEAR_ALLY,
};

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

//Card class
class Card {
public:
	Card() {}
	~Card() {}
	virtual void drawDetails(Renderer& rm, int& y) {}
	int cost;
	eCard type;
	std::string name;
	Sprite sprite;
	Sprite header[2];
};

//Unit class
class Unit : public Card {
public:
	Unit(eFaction = FACTION_NEUTRAL, eTribe = TRIBE_NONE, int = 0, int = 0, int = 0, std::string = "", std::string = "???");
	~Unit();
	void render(Renderer& rm);
	void attack(Unit& u);
	void setPos(int x, int y);
	void update(bool& r);
	void updateStatSprites();
	void updateDetailStats();
	void generateDetails();
	void drawDetails(Renderer& rm, int& y);
	void onSummon(Unit& u);
	void onDeath(Unit& u);
	void onAttack(Unit& u1, Unit& u2);
	eFaction faction;
	eTribe tribe;
	int atk;
	int hp;
	int hpMax;
	BoardTile* tile;
	Game* game;
	Player* player;
	std::vector<Effect> effect;
	std::vector<Buff> buff;
	Sprite sHP;
	Sprite sATK;
};

//Card list class
class CardList {
public:
	CardList();
	~CardList();
	Card* find(std::string s);
	EffectList el;
	std::vector<Card*> clist;
	std::vector<Unit> glist;
	std::vector<Unit> ulist;
};

#endif
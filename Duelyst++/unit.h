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

//Effects
enum eEffect {
	EFFECT_NONE
};

//Granted stat buffs
enum eBuff {
	BUFF_NONE
};

//Effect container
class Effect {
public:
	Effect();
	~Effect();
	Sprite sprite;
	eEffect effect;
};

//Stat buff container
class Buff {
public:
	Buff();
	~Buff();
	Sprite sprite;
	eBuff buff;
	int atk;
	int hp;
	int count;
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
	BoardTile* tile;
	Game* game;
	Player* player;
	Effect skill;
	std::vector<Effect> effect;
	std::vector<Buff> buff;
	Sprite sHP;
	Sprite sATK;
	std::string cardKeywords;
	std::vector<std::string> effectName;
	std::vector<std::string> effectDescription;
};

#endif
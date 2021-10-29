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
	TARGET_MINION_NEAR_UNIT,
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
	virtual void onSummon(Unit* u) {}
	virtual void onDeath(Unit* u) {}
	virtual void onAttack(Unit* u1, Unit* u2) {}
	virtual void onDamage(Unit* u1, Unit* u2) {}
	virtual bool onReplace() { return true; }
	virtual void onTurnEnd(Player* p) {}
	virtual void onTurnStart(Player* p) {}
	eFaction faction;
	eCard type;
	int cost;
	Game* game;
	Player* player;
	Card* original;
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
	void setPos(int x, int y);
	void addBuff(eBuff b);
	void removeBuff(eBuff b);
	void addEffect(eEffect e);
	void removeEffect(eEffect e);
	void update(bool& r);
	void updateStatBuffs();
	void updateStatSprites();
	void updateDetailStats();
	void generateDetails();
	void drawDetails(Renderer& rm, int& y);
	bool canAttack(Unit* u);
	bool isMoveable();
	bool isFlying();
	void attack(Unit* u, bool counter);
	void onSummon(Unit* u);
	void onDeath(Unit* u);
	void onAttack(Unit* u1, Unit* u2);
	void onDamage(Unit* u1, Unit* u2);
	bool onReplace();
	void onTurnEnd(Player* p);
	void onTurnStart(Player* p);
	void callback(BoardTile* t);
	eTribe tribe;
	int atk;
	int hp;
	int hpMax;
	bool dead;
	bool moved;
	bool attacked;
	BoardTile* tile;
	Skill skill;
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
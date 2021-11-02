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
	TARGET_TILE,
	TARGET_UNIT,
	TARGET_MINION,
	TARGET_GENERAL,
	TARGET_ALLY,
	TARGET_ALLY_MINON,
	TARGET_ALLY_GENERAL,
	TARGET_ENEMY,
	TARGET_ENEMY_MINION,
	TARGET_ENEMY_RANGED,
	TARGET_ENEMY_GENERAL,
	TARGET_NEAR_ANY,
	TARGET_NEAR_UNIT,
	TARGET_MINION_NEAR_UNIT,
	TARGET_NEAR_ALLY,
	TARGET_NEAR_ENEMY
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
	TRIBE_STRUCTURE,
	TRIBE_WARMASTER
};

//Card class
class Card {
public:
	Card() {
		token = nullptr;
		divider.createFromString("컴TOKEN컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴");
	}
	~Card() {}
	virtual void drawDetails(Renderer& rm, int& y) {}
	virtual void onSummon(Unit* u, bool actionBar) {}
	virtual void onDeath(Unit* u) {}
	virtual void onAttack(Unit* u1, Unit* u2) {}
	virtual void onDamage(Unit* u1, Unit* u2, int damage) {}
	virtual void onHeal(Unit* u1, Unit* u2, int heal) {}
	virtual void onMove(Unit* u, bool byEffect) {}
	virtual void onDraw(Card* c, bool fromDeck) {}
	virtual bool onReplace() { return true; }
	virtual void onTurnEnd(Player* p) {}
	virtual void onTurnStart(Player* p) {}
	eFaction faction;
	eCard type;
	int cost;
	Game* game;
	Player* player;
	Card* original;
	Card* token;
	std::string name;
	Sprite sprite;
	Sprite header[2];
	Sprite divider;
};

//Unit class
class Unit : public Card {
public:
	Unit(eFaction = FACTION_NEUTRAL, eTribe = TRIBE_NONE, int = 0, int = 0, int = 0, std::string = "", std::string = "???");
	~Unit();
	void render(Renderer& rm);
	void setPos(int x, int y);
	void addBuff(eBuff b);
	void removeBuff(eBuff b, bool allStacks);
	void addEffect(eEffect e);
	void removeEffect(eEffect e, bool allStacks);
	void update(bool& r);
	void updateStatBuffs();
	void updateStatSprites();
	void generateDetails();
	void updateDetailStats();
	void drawDetails(Renderer& rm, int& y);
	bool canAttack(Unit* u);
	bool isMoveable();
	bool isFlying();
	bool isRanged();
	bool isProvoking();
	bool isProvoked();
	void attack(Unit* u, bool counter);
	int dealDamage(Unit* u, int damage);
	void onSummon(Unit* u, bool actionBar);
	void onDeath(Unit* u);
	void onAttack(Unit* u1, Unit* u2);
	void onDamage(Unit* u1, Unit* u2, int damage);
	void onHeal(Unit* u1, Unit* u2, int heal);
	void onMove(Unit* u, bool byEffect);
	void onDraw(Card* c, bool fromDeck);
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

//Spell class
class Spell : public Card {
public:
	Spell(eFaction = FACTION_NEUTRAL,  eTarget = TARGET_ANY, int = 0, std::string = "", std::string = "???");
	~Spell();
	void addBuff(eBuff b);
	void removeBuff(eBuff b, bool allStacks);
	void updateStatBuffs();
	void generateDetails();
	void updateDetailStats();
	void drawDetails(Renderer& rm, int& y);
	void onUse(BoardTile* t);
	void callback(BoardTile* t);
	void lateCallback();
	eTarget target;
	SpellEffect spell;
	std::vector<Buff> buff;
};

//Card list class
class CardList {
public:
	CardList();
	~CardList();
	Card* find(std::string s);
	EffectList el;
	std::vector<Card*> cList;
	std::vector<Unit> gList;
	std::vector<Unit> uList;
	std::vector<Spell> sList;
};

#endif
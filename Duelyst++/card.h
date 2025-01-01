//Defines
#ifndef __CARD_H__
#define __CARD_H__

//Include
#include <algorithm>
#include "effect.h"

//Definitions
class Game;
class Player;
class Spell;

#pragma region Enums / Helpers

//Card types
enum eCard {
	CARD_NONE,
	CARD_MINION,
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

//Targeting modes
enum eTargetMode {
	TARGET_MODE_ALL,
	TARGET_MODE_NEAR_TILE,
	TARGET_MODE_NEAR_UNITS,
	TARGET_MODE_NEAR_ALLIES,
	TARGET_MODE_NEAR_ENEMIES
};

//Targeting filters
enum eTargetFilters {
	TARGET_FILTER_NONE    = 0,
	TARGET_FILTER_EMPTY   = 1 << 0,
	TARGET_FILTER_UNIT    = 1 << 1,
	TARGET_FILTER_MINION  = 1 << 2,
	TARGET_FILTER_GENERAL = 1 << 3,
	TARGET_FILTER_ALLY    = 1 << 4,
	TARGET_FILTER_ENEMY   = 1 << 5
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

//Targeting mode
class TargetMode {
public:
	TargetMode();
	TargetMode(eTargetMode _mode, int _filters);
	~TargetMode();
	bool HasFilters(int flags) { return (filters & flags) == flags; }
	bool HasAny(int flags) { return (filters & flags) != TARGET_FILTER_NONE; }
	eTargetMode mode;
	int filters;
};

#pragma endregion

//Card class
class Card {
public:

	//Constructors
	Card();
	~Card();

	//Effects
	void AddEffect(Effect effect, Card* source);
	void RemoveEffect(Effect effect, Card* source, bool allStacks);

	//Actions
	void PreCast(BoardTile* tile);
	virtual void Resolve(BoardTile* tile);

	//Events
	virtual void OnCast(Card* card, BoardTile* tile);
	virtual void OnSummon(Minion* minion, bool fromActionBar);
	virtual void OnDeath(Minion* minion);
	virtual void OnAttack(Minion* source, Minion* target, bool counter);
	virtual void OnDamage(Card* source, Minion* target, int damage);
	virtual void OnHeal(Card* source, Minion* target, int heal);
	virtual void OnMove(Minion* minion, bool byEffect);
	virtual void OnDraw(Card* card, bool fromDeck);
	virtual void OnReplace(Card* replaced);
	virtual void OnTurnStart(Player* player);
	virtual void OnTurnEnd(Player* player);

	//Virtual functions
	virtual void UpdateStatBuffs() {}
	virtual void DrawDetails(Renderer& renderer, int& y) {}

	//Subclass getters
	virtual Minion* GetMinion() { return nullptr; }
	virtual Spell* GetSpell() { return nullptr; }
	bool IsMinion() { return GetMinion() != nullptr; }
	bool IsSpell() { return GetSpell() != nullptr; }

	//Properties
	eFaction faction;
	eCard cardType;
	TargetMode targetMode;
	bool isToken;
	int cost;
	Game* game;
	Player* owner;
	Card* original;
	Card* token;
	std::string name;
	std::vector<Effect> effects;
	Sprite sprite;
	Sprite header[2];
	Sprite divider;

};

//Minion class
class Minion : public Card {
public:

	//Constructors / Initialization
	Minion();
	Minion(eFaction _faction, eTribe _tribe, int _cost, int _atk, int _hp, std::string path, std::string _name);
	Minion(eFaction _faction, eTribe _tribe, int _cost, int _atk, int _hp, std::string path, std::string _name, Effect effect);
	~Minion();
	void GenerateDetails();

	//Rendering
	void Render(Renderer& renderer);
	void DrawDetails(Renderer& renderer, int& y);

	//Updates
	void Update(bool& shouldLoop);
	void UpdateStatBuffs();
	void UpdateStatSprites();
	void UpdateDetailStats();

	//Actions
	void SetPosition(int x, int y);
	void Attack(Minion* target, bool counter);
	int DealDamage(Minion* source, int damage);
	void Dispel();

	//Utils
	bool CanAttack(Minion* target);
	bool IsMoveable();
	int MoveRange();
	bool HasKeywords(int keywords);
	bool IsProvoked();

	//Action & Event Overrides
	void Resolve(BoardTile* tile);
	void OnTurnEnd(Player* player);

	//Getter
	Minion* GetMinion() { return this; }

	//Properties
	eTribe tribe;
	int atk;
	int hp;
	int hpMax;
	bool isDead;
	bool hasMoved;
	bool hasAttacked;
	bool hasCelerityMoved;
	bool hasCelerityAttacked;
	bool hasForcefield;
	BoardTile* curTile;
	Sprite hpSprite;
	Sprite atkSprite;

};

//Spell class
class Spell : public Card {
public:

	//Constructors / Initialization
	Spell();
	Spell(eFaction _faction, TargetMode _targetMode, int _cost, std::string path, std::string _name);
	Spell(eFaction _faction, TargetMode _targetMode, int _cost, std::string path, std::string _name, Effect effect);
	~Spell();
	void GenerateDetails();

	//Rendering
	void DrawDetails(Renderer& renderer, int& y);

	//Updates
	void UpdateStatBuffs();
	void UpdateDetailStats();

	//Action & Event Overrides
	void Resolve(BoardTile* tile);

	//Getter
	Spell* GetSpell() { return this; }

};

#endif
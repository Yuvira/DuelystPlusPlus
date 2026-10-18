#pragma once

//Include
#include "cardids.h"
#include "effectids.h"
#include "sprite.h"
#include "modifiers.h"

//Definitions
class Renderer;
class Game;
class BoardTile;
class Effect;
class Player;
class Unit;
class Spell;
struct Event;

#pragma region Targeting Helpers

//Targeting modes
enum eTargetMode {
	TARGET_MODE_ALL,
	TARGET_MODE_NEAR_TILE,
	TARGET_MODE_NEAR_UNITS,
	TARGET_MODE_NEAR_ALLIES,
	TARGET_MODE_NEAR_ENEMIES
};

//Targeting mode
struct TargetMode {
	TargetMode();
	TargetMode(eTargetMode mode, bool (*Predicate)(BoardTile*));
	~TargetMode();
	eTargetMode mode;
	bool (*Predicate)(BoardTile*);
};

#pragma endregion

//Card class
class Card {
public:

	//Constructors
	Card();
	~Card();

	//Detail rendering
	void UpdateDetails();
	virtual void DrawDetails(Renderer& renderer, int& y) {}

	//Stats
	virtual void UpdateModifiers() {}
	int GetStat(eStat stat, int base);

	//Effects
	void AddEffect(eEffect effectId, Card* refCard);
	void AddEffect(Effect effect, Card* refCard);
	void RemoveEffect(Effect* effect);
	void RemoveEffectAt(std::vector<int> indices);
	void UpdateAuras();
	bool HasAuraFromSource(Effect* source);

	//Getters
	std::vector<Effect*> GetEffects();

	//Utils
	virtual TargetMode GetTargetMode() { return TargetMode(); }
	virtual bool IsGeneral() { return false; }
	virtual bool IsOnBoard() { return false; }
	bool IsAlly();
	bool IsEnemy();

	//Subclass getters
	virtual Unit* GetUnit() { return nullptr; }
	virtual Spell* GetSpell() { return nullptr; }
	bool IsUnit() { return GetUnit() != nullptr; }
	bool IsSpell() { return GetSpell() != nullptr; }

	//Properties
	eCard cardId;
	eFaction faction;
	bool isToken;
	int baseCost;
	int cost;
	Game* game;
	Player* owner;
	Card* original;
	std::string name;
	std::vector<Effect*> effects;
	std::vector<Effect*> auras;
	Sprite sprite;
	Sprite header[2];
	Sprite details;
	Sprite divider;

};

//Unit class
class Unit : public Card {
public:

	//Constructors / Initialization
	Unit();
	Unit(eCard cardId, eFaction faction, eTribe tribe, int cost, int atk, int hp, std::string path, std::string name);
	Unit(eCard cardId, eFaction faction, eTribe tribe, int cost, int atk, int hp, std::string path, std::string name, bool isToken);
	Unit(eCard cardId, eFaction faction, eTribe tribe, int cost, int atk, int hp, std::string path, std::string name, Effect* effect);
	Unit(eCard cardId, eFaction faction, eTribe tribe, int cost, int atk, int hp, std::string path, std::string name, bool isToken, Effect* effect);
	~Unit();
	void GenerateDetails();

	//Rendering
	void Render(Renderer& renderer);
	void DrawDetails(Renderer& renderer, int& y);
	void UpdateStatSprites();
	void UpdateDetailStats();

	//Updates
	void Update(bool& shouldLoop);
	void UpdateModifiers();

	//Actions
	void InitState();
	void SetPosition(int x, int y);
	void MoveToTile(BoardTile* tile, bool byEffect);
	void MoveToPosition(int x, int y, bool byEffect);
	void Attack(Unit* target, bool counter);
	void Exhaust();
	void Refresh();
	int DealDamage(Card* source, int damage);
	int Heal(Card* source, int heal);
	void Destroy(Card* source);
	void Dispel();
	void AddEffects();
	void RemoveEffects();

	//Utils
	TargetMode GetTargetMode();
	bool IsGeneral() { return tribe == TRIBE_GENERAL; }
	bool CanAttack(Unit* target);
	bool IsMoveable();
	bool HasKeywords(int match);
	bool IsProvoked();
	bool IsOnBoard() { return curTile != nullptr; }

	//Getter
	Unit* GetUnit() { return this; }

	//Properties
	eTribe tribe;
	int baseAtk;
	int atk;
	int hp;
	int baseHpMax;
	int hpMax;
	int baseMoveRange;
	int moveRange;
	int keywords;
	bool destroyed;
	bool isDead;
	bool hasMoved;
	bool hasAttacked;
	bool hasCelerityMoved;
	bool hasCelerityAttacked;
	bool forcefieldBroken;
	BoardTile* curTile;
	Sprite hpSprite;
	Sprite atkSprite;

};

//Spell class
class Spell : public Card {

//Methods
public:

	//Constructors / Initialization
	Spell();
	Spell(eCard cardId, eFaction faction, int cost, std::string path, std::string name);
	Spell(eCard cardId, eFaction faction, int cost, std::string path, std::string name, bool isToken);
	Spell(eCard cardId, eFaction faction, int cost, std::string path, std::string name, Effect* effect);
	Spell(eCard cardId, eFaction faction, int cost, std::string path, std::string name, bool isToken, Effect* effect);
	~Spell();
	void GenerateDetails();

	//Rendering
	void DrawDetails(Renderer& renderer, int& y);
	void UpdateDetailStats();

	//Updates
	void UpdateModifiers();

	//Getters
	TargetMode GetTargetMode() { return targetMode; }
	Spell* GetSpell() { return this; }

	//Setters
	void SetTargetMode(TargetMode targetMode);

//Properties
private:

	//Targeting mode
	TargetMode targetMode;

};
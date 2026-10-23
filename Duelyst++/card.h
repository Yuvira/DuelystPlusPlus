#pragma once

//Include
#include "cardids.h"
#include "effectids.h"
#include "sprite.h"
#include "modifiers.h"

//Definitions
class Aura;
class BoardTile;
class CardDefinition;
class Effect;
class Game;
class Player;
class Renderer;
class Spell;
class SpellEffect;
class Unit;
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

	//Constructor
	Card(const CardDefinition& definition, Game* game, Player* owner);
	~Card();

	//Rendering
	virtual void DrawDetails(Renderer& renderer, int& y);
	void UpdateDetails();

	//Stats
	virtual void UpdateModifiers() = 0;
	int GetStat(eStat stat, int base, bool withAuras);

	//Targeting
	virtual std::vector<BoardTile*> GetTargets() = 0;

	//Effects
	Effect* AddEffect(eEffect effectId);
	void RemoveEffect(Effect* effect);
	void RemoveEffectAt(std::vector<int> indices);
	void UpdateAuras();
	bool HasAuraFromSource(Effect* source);

	//Getters
	std::vector<Effect*> GetEffects();
	std::vector<eCard> GetTokens();

	//Readonly getters
	const eCard& ReadID() const { return cardId; }
	const Sprite& ReadSprite() const { return sprite; }
	const Sprite& ReadDetailNameSprite() const { return detailNameSprite; }
	const Sprite& ReadDetailStatSprite() const { return detailStatSprite; }
	const std::vector<std::string>& ReadDetails() const { return details; }

	//Subclass getters and helpers
	virtual Unit* GetUnit() { return nullptr; }
	virtual Spell* GetSpell() { return nullptr; }
	bool IsUnit() { return GetUnit() != nullptr; }
	bool IsSpell() { return GetSpell() != nullptr; }
	virtual bool IsGeneral() { return false; }
	virtual bool IsMinion() { return false; }
	virtual bool IsOnBoard() { return false; }

	//Properties
	eCard cardId;
	eRarity rarity;
	eFaction faction;
	std::string name;
	int baseCost;
	int cost;
	Game* game;
	Player* owner;
	std::vector<Effect*> effects;
	std::vector<Aura*> auras;
	bool isToken;
	Sprite sprite;
	Sprite detailNameSprite;
	Sprite detailStatSprite;
	std::vector<std::string> details;

};

//Unit class
class Unit : public Card {

public:

	//Constructor
	Unit(const CardDefinition& definition, Game* game, Player* owner);
	~Unit();

	//Rendering
	void RenderOnBoard(Renderer& renderer);
	void RenderOnBoard(Renderer& renderer, int x, int y);
	void DrawDetails(Renderer& renderer, int& y);

	//Updates
	void Update(bool& shouldLoop);
	void UpdateModifiers();

	//Actions
	std::vector<BoardTile*> GetTargets();
	void InitState();
	void SetTile(BoardTile* tile);
	void MoveToTile(BoardTile* tile, bool byEffect);
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
	bool CanAttack(Unit* target);
	bool IsMoveable();
	bool HasKeywords(int match);
	bool IsProvoked();

	//Subclass getter and helpers
	Unit* GetUnit() { return this; }
	bool IsGeneral() { return tribe == TRIBE_GENERAL; }
	bool IsMinion() { return tribe != TRIBE_GENERAL; }
	bool IsOnBoard() { return curTile != nullptr; }

	//Properties
	eTribe tribe;
	int baseAtk;
	int atk;
	int baseHpMax;
	int hpMax;
	int hp;
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

public:

	//Constructor
	Spell(const CardDefinition& definition, Game* game, Player* owner);
	~Spell();

	//Rendering
	void DrawDetails(Renderer& renderer, int& y);

	//Updates
	void UpdateModifiers();

	//Targeting
	std::vector<BoardTile*> GetTargets();

	//Getters
	Spell* GetSpell() { return this; }

private:

	//Spell effect
	SpellEffect* spellEffect;

};
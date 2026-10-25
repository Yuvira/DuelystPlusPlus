#pragma once

//Include
#include "cardids.h"
#include "effectids.h"
#include "sprite.h"
#include "modifiers.h"

//Definitions
class Aura;
class BoardTile;
class BoardCard;
class CardDefinition;
class Effect;
class Game;
class Player;
class Renderer;
class Spell;
class SpellEffect;
class Tile;
class Unit;
struct Event;

//Card class
class Card {

public:

	//Constructor and virtual destructor
	explicit Card(const CardDefinition& definition, Game* game, Player* owner);
	virtual ~Card() = 0;

	//Rendering
	virtual void DrawDetails(Renderer& renderer, int& y);
	virtual void UpdateDetails();

	//Stats
	virtual void UpdateModifiers() {}

	//Effects
	virtual Effect* AddEffect(eEffect effectId);
	virtual bool RemoveEffect(Effect* effect);

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
	virtual Tile* GetTile() { return nullptr; }
	bool IsUnit() { return GetUnit() != nullptr; }
	bool IsSpell() { return GetSpell() != nullptr; }
	bool IsTile() { return GetTile() != nullptr; }
	virtual bool IsGeneral() { return false; }
	virtual bool IsMinion() { return false; }
	virtual bool IsOnBoard() { return false; }
	virtual BoardCard* GetCardOnBoard() { return nullptr; }

	//Properties
	eCard cardId;
	eRarity rarity;
	eFaction faction;
	std::string name;
	Game* game;
	Player* owner;
	std::vector<Effect*> effects;
	Sprite sprite;
	Sprite detailNameSprite;
	Sprite detailStatSprite;
	std::vector<std::string> details;

};

//Cards that can exist in hand/deck and be played
class PlayableCard : public virtual Card {

public:

	//Virtual destructor
	~PlayableCard() override = 0;

	//Rendering
	void UpdateDetails() override;

	//Stats
	virtual void UpdateModifiers() override = 0;
	virtual int GetStat(eStat stat, bool withAuras) = 0;

	//Targeting
	virtual std::vector<BoardTile*> GetTargets() = 0;

	//Effects
	Effect* AddEffect(eEffect effectId) override;
	bool RemoveEffect(Effect* effect) override;
	void UpdateAuras();
	bool HasAuraFromSource(Effect* source);

	//Properties
	int baseCost = 0;
	int cost = 0;
	std::vector<Aura*> auras;
	bool isToken = false;

protected:

	//Internal stat getter
	int GetStat(eStat stat, int base, bool withAuras);

};

//Cards that can exist on the board
class BoardCard : public virtual Card {

public:

	//Virtual destructor
	~BoardCard() override = 0;

	//Utils
	bool IsOnBoard() override { return curTile != nullptr; }
	BoardCard* GetCardOnBoard() override { return this; }

	//Properties
	BoardTile* curTile = nullptr;

};

//Unit class
class Unit : public PlayableCard, public BoardCard {

public:

	//Constructor
	Unit(const CardDefinition& definition, Game* game, Player* owner);
	~Unit() override;

	//Rendering
	void RenderOnBoard(Renderer& renderer, int x, int y);
	void DrawDetails(Renderer& renderer, int& y) override;

	//Updates & Stats
	void Update(bool& shouldLoop);
	void UpdateModifiers() override;
	int GetStat(eStat stat, bool withAuras) override;

	//Actions
	std::vector<BoardTile*> GetTargets() override;
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
	bool HasKeyword(eKeywordFlags match) { return HasAllKeywords(match); }
	bool HasAllKeywords(int match);
	bool HasAnyKeywords(int match);
	bool IsProvoked();

	//Subclass getter and helpers
	Unit* GetUnit() override { return this; }
	bool IsGeneral() override { return tribe == TRIBE_GENERAL; }
	bool IsMinion() override { return tribe != TRIBE_GENERAL; }

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
	Sprite hpSprite;
	Sprite atkSprite;

};

//Spell class
class Spell : public PlayableCard {

public:

	//Constructor
	Spell(const CardDefinition& definition, Game* game, Player* owner);
	~Spell() override;

	//Rendering
	void DrawDetails(Renderer& renderer, int& y) override;

	//Updates & Stats
	void UpdateModifiers() override;
	int GetStat(eStat stat, bool withAuras) override;

	//Targeting
	std::vector<BoardTile*> GetTargets() override;

	//Getters
	SpellEffect* GetSpellEffect() { return spellEffect; }
	Spell* GetSpell() override { return this; }

private:

	//Spell effect
	SpellEffect* spellEffect;

};

//Tile class
class Tile : public BoardCard {

public:

	//Constructor
	Tile(const CardDefinition& definition, Game* game, Player* owner);
	~Tile() override;

	//On enter/leave events
	void AddEffects();
	void RemoveEffects();
	void TriggerUnitChanged();

	//Subclass getter
	Tile* GetTile() override { return this; }

};
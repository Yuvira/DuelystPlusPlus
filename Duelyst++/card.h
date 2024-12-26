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

#pragma region Enums

//Card types
enum eCard {
	CARD_NONE,
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
	TARGET_TILE_NEAR_UNIT,
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

#pragma endregion

//Card class
class Card {
public:
	Card();
	~Card();
	void AddEffect(Effect effect, Card* source);
	void RemoveEffect(Effect effect, Card* source, bool allStacks);
	virtual void OnCast(Card* card, BoardTile* tile);
	virtual void UpdateStatBuffs() {}
	virtual void DrawDetails(Renderer& renderer, int& y) {}
	virtual void OnSummon(Minion* minion, bool fromActionBar) {}
	virtual void OnDeath(Minion* minion) {}
	virtual void OnAttack(Minion* source, Minion* target, bool counter) {}
	virtual void OnDamage(Minion* source, Minion* target, int damage) {}
	virtual void OnHeal(Minion* source, Minion* target, int heal) {}
	virtual void OnMove(Minion* minion, bool byEffect) {}
	virtual void OnSpellCast(Spell* spell) {}
	virtual void OnDraw(Card* card, bool fromDeck) {}
	virtual void OnReplace(Card* replaced) {}
	virtual void OnTurnEnd(Player* player) {}
	virtual void OnTurnStart(Player* player) {}
	eFaction faction;
	eCard cardType;
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
	Minion();
	Minion(eFaction _faction, eTribe _tribe, int _cost, int _atk, int _hp, std::string path, std::string _name);
	Minion(eFaction _faction, eTribe _tribe, int _cost, int _atk, int _hp, std::string path, std::string _name, Effect effect);
	~Minion();
	void GenerateDetails();
	void Render(Renderer& renderer);
	void DrawDetails(Renderer& renderer, int& y);
	void Update(bool& shouldLoop);
	void UpdateStatBuffs();
	void UpdateStatSprites();
	void UpdateDetailStats();
	void SetPosition(int x, int y);
	void Attack(Minion* target, bool counter);
	int DealDamage(Minion* source, int damage);
	void Dispel();
	bool CanAttack(Minion* target);
	bool IsMoveable();
	int MoveRange();
	bool HasKeywords(eKeywordFlags keywords);
	bool IsProvoked();
	void OnCast(Card* card, BoardTile* tile);
	void OnSummon(Minion* minion, bool actionBar);
	void OnDeath(Minion* minion);
	void OnAttack(Minion* source, Minion* target, bool counter);
	void OnDamage(Minion* source, Minion* target, int damage);
	void OnHeal(Minion* source, Minion* target, int heal);
	void OnMove(Minion* minion, bool byEffect);
	void OnSpellCast(Spell* spell);
	void OnDraw(Card* card, bool fromDeck);
	void OnReplace(Card* replaced);
	void OnTurnEnd(Player* player);
	void OnTurnStart(Player* player);
	void Callback(BoardTile* tile);
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
	Spell();
	Spell(eFaction _faction, eTarget _target, int _cost, std::string path, std::string _name);
	~Spell();
	void GenerateDetails();
	void DrawDetails(Renderer& renderer, int& y);
	void UpdateStatBuffs();
	void UpdateDetailStats();
	void OnCast(Card* card, BoardTile* tile);
	void OnUse(BoardTile* tile);
	void Callback(BoardTile* tile);
	void LateCallback();
	eTarget targetMode;
	SpellEffect spell;
};

//Card list class
class CardList {
public:
	CardList();
	~CardList();
	Card* FindCard(std::string name);
	Effect FindEffect(eEffect effect);
	EffectList effectList;
	std::vector<Card*> cardList;
	std::vector<Minion> generalList;
	std::vector<Minion> minionList;
	std::vector<Minion> minionTokenList;
	std::vector<Spell> spellList;
	std::vector<Spell> spellTokenList;
};

#endif
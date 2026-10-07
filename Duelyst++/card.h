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

#pragma region Card List

//Card identifiers
enum eCard {
	CARD_NONE,
	CARD_ABJUDICATOR,
	CARD_AETHERMASTER,
	CARD_ALCUIN_LOREMASTER,
	CARD_ARAKI_HEADHUNTER,
	CARD_ARCHON_SPELLBINDER,
	CARD_ARGEON_HIGHMAYNE,
	CARD_ARROW_WHISTLER,
	CARD_ASH_MEPHYT,
	CARD_ASTRAL_CRUSADER,
	CARD_AZURE_HERALD,
	CARD_AZURE_HORN_SHAMAN,
	CARD_BASTION,
	CARD_BLACK_LOCUST,
	CARD_BLAZE_HOUND,
	CARD_BLISTERING_SKORN,
	CARD_BLOOD_TAURA,
	CARD_BLOODSHARD_GOLEM,
	CARD_BLOODTEAR_ALCHEMIST,
	CARD_BLUETIP_SCORPION,
	CARD_BREATH_OF_THE_UNBORN,
	CARD_BRIGHTMOSS_GOLEM,
	CARD_DAGGER_KIRI,
	CARD_DARK_SEED,
	CARD_DIAMOND_GOLEM,
	CARD_DRAGONLARK,
	CARD_DRYBONE_GOLEM,
	CARD_FACESTRIKER,
	CARD_FIREBLAZER,
	CARD_FIRE_SPITTER,
	CARD_HAILSTONE_GOLEM,
	CARD_KOMODO_CHARGER,
	CARD_SABERSPINE_TIGER,
	CARD_SAPPHIRE_SEER,
	CARD_EPHEMERAL_SHROUD
};

#pragma endregion

//Card types
enum eCardType {
	CARDTYPE_NONE,
	CARDTYPE_MINION,
	CARDTYPE_SPELL,
	CARDTYPE_ARTIFACT
};

//Card tags
enum eCardTag {
	CARDTAG_NONE,
	CARDTAG_GENERAL,
	CARDTAG_TOKEN
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
	TargetMode(eTargetMode mode, int filters);
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

	//Updates & Rendering
	void UpdateDetails();
	virtual void UpdateStatBuffs() {}
	virtual void DrawDetails(Renderer& renderer, int& y) {}

	//Effects
	void AddEffect(Effect effect, Effect* source);
	void RemoveEffect(Effect* effect);
	void RemoveEffectsFromSource(Effect* source);
	void RemoveEffectAt(std::vector<int> indices);

	//Actions
	void PreCast(BoardTile* tile);
	virtual void Resolve(BoardTile* tile);

	//Events
	void OnCast(Card* card, BoardTile* tile);
	void OnSummon(Minion* minion, bool fromActionBar);
	void OnDeath(Minion* minion);
	void OnAttack(Minion* source, Minion* target, int& damage, bool counter);
	void OnDamage(Card* source, Minion* target, int damage);
	void OnHeal(Card* source, Minion* target, int heal);
	void OnMove(Minion* minion, bool byEffect);
	void OnDraw(Card* card, bool fromDeck);
	void OnReplace(Card* replaced);
	void OnEffectsChanged(Card* card);
	void OnTurnStart(Player* player);
	virtual void OnTurnEnd(Player* player);

	//Utils
	virtual bool IsOnBoard() { return false; }
	std::string ValueString(int value);
	int TextWidth(std::string str);

	//Subclass getters
	virtual Minion* GetMinion() { return nullptr; }
	virtual Spell* GetSpell() { return nullptr; }
	bool IsMinion() { return GetMinion() != nullptr; }
	bool IsSpell() { return GetSpell() != nullptr; }

	//Properties
	eCard cardId;
	eCardTag cardTag;
	eCardType cardType;
	eFaction faction;
	TargetMode targetMode;
	bool isToken;
	int cost;
	Game* game;
	Player* owner;
	Card* original;
	Card* token;
	std::string name;
	std::vector<Effect*> effects;
	Sprite sprite;
	Sprite header[2];
	Sprite details;
	Sprite divider;

};

//Minion class
class Minion : public Card {
public:

	//Constructors / Initialization
	Minion();
	Minion(eCard cardId, eCardTag cardTag, eFaction faction, eTribe tribe, int cost, int atk, int hp, std::string path, std::string name);
	Minion(eCard cardId, eCardTag cardTag, eFaction faction, eTribe tribe, int cost, int atk, int hp, std::string path, std::string name, Effect effect);
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
	int DealDamage(Card* source, int damage);
	void Dispel();
	void AddEffects();

	//Utils
	bool CanAttack(Minion* target);
	bool IsMoveable();
	int MoveRange();
	bool HasKeywords(int keywords);
	bool IsProvoked();
	bool IsOnBoard() { return curTile != nullptr; }

	//Action & Event Overrides
	void Resolve(BoardTile* tile);
	void InitState();
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
	Spell(eCard cardId, eCardTag cardTag, eFaction faction, TargetMode targetMode, int cost, std::string path, std::string name);
	Spell(eCard cardId, eCardTag cardTag, eFaction faction, TargetMode targetMode, int cost, std::string path, std::string name, Effect effect);
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
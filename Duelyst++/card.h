#pragma once

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
	CARD_BONEREAPER,
	CARD_BREATH_OF_THE_UNBORN,
	CARD_BRIGHTMOSS_GOLEM,
	CARD_CAPTAIN_HANK_HART,
	CARD_CHAKKRAM,
	CARD_CHAOS_ELEMENTAL,
	CARD_CRIMSON_OCULUS,
	CARD_CROSSBONES,
	CARD_DAGGER_KIRI,
	CARD_DANCING_BLADES,
	CARD_DARK_NEMESIS,
	CARD_DARK_SEED,
	CARD_DAY_WATCHER,
	CARD_DEATHBLIGHTER,
	CARD_DECIMUS,
	CARD_DIAMOND_GOLEM,
	CARD_DIOLTAS,
	CARD_DRAGONLARK,
	CARD_DREAMGAZER,
	CARD_DRYBONE_GOLEM,
	CARD_DUST_WAILER,
	CARD_ECLIPSE,
	CARD_EMERALD_REJUVENATOR,
	CARD_ENVYBAER,
	CARD_EPHEMERAL_SHROUD,
	CARD_EXUN,
	CARD_FACESTRIKER,
	CARD_FIREBLAZER,
	CARD_FIRESTARTER,
	CARD_FIRE_SPITTER,
	CARD_FIRST_SWORD_OF_AKRANE,
	CARD_FLAMEBLOOD_WARLOCK,
	CARD_FROSTBONE_NAGA,
	CARD_GHOST_LYNX,
	CARD_GOLDEN_JUSTICAR,
	CARD_GOLEM_METALLURGIST,
	CARD_GOLEM_VANQUISHER,
	CARD_GROVE_LION,
	CARD_HAILSTONE_GOLEM,
	CARD_HEALING_MYSTIC,
	CARD_IRONCLAD,
	CARD_JAX_TRUESIGHT,
	CARD_JAXI,
	CARD_KEEPER_OF_THE_VALE,
	CARD_KHYMERA,
	CARD_KOMODO_CHARGER,
	CARD_LADY_LOCKE,
	CARD_LIGHTBENDER,
	CARD_LUX_IGNIS,
	CARD_MANAFORGER,
	CARD_MINI_JAX,
	CARD_PIERCING_MANTIS,
	CARD_SABERSPINE_TIGER,
	CARD_SAPPHIRE_SEER,
	CARD_SPELLSPARK,
	CARD_TOMBSTONE
};

#pragma endregion

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

	//Updates & Rendering
	void UpdateDetails();
	virtual void UpdateStatBuffs() {}
	virtual void DrawDetails(Renderer& renderer, int& y) {}

	//Effects
	void AddEffect(Effect effect, Effect* source);
	void AddContinuousEffect(Effect effect);
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
	void OnAttack(Minion* source, Minion* target, bool counter);
	void OnWouldDealDamage(Card* source, Minion* target, int& damage);
	void OnDamageDealt(Card* source, Minion* target, int damage);
	void OnWouldHeal(Card* source, Minion* target, int& heal);
	void OnHealed(Card* source, Minion* target, int heal);
	void OnMove(Minion* minion, bool byEffect);
	void OnDraw(Card* card, bool fromDeck);
	void OnReplace(Card* replaced, bool& sendToDeck);
	void OnEffectsChanged(Card* card);
	void OnTurnStart(Player* player);
	virtual void OnTurnEnd(Player* player);

	//Utils
	virtual bool IsOnBoard() { return false; }
	std::string ValueString(int value);
	int TextWidth(std::string str);
	bool IsAlly();
	bool IsEnemy();

	//Subclass getters
	virtual Minion* GetMinion() { return nullptr; }
	virtual Spell* GetSpell() { return nullptr; }
	bool IsMinion() { return GetMinion() != nullptr; }
	bool IsSpell() { return GetSpell() != nullptr; }

	//Properties
	eCard cardId;
	eFaction faction;
	TargetMode targetMode;
	bool isToken;
	int cost;
	Game* game;
	Player* owner;
	Card* original;
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
	Minion(eCard cardId, eFaction faction, eTribe tribe, int cost, int atk, int hp, std::string path, std::string name);
	Minion(eCard cardId, eFaction faction, eTribe tribe, int cost, int atk, int hp, std::string path, std::string name, bool isToken);
	Minion(eCard cardId, eFaction faction, eTribe tribe, int cost, int atk, int hp, std::string path, std::string name, Effect* effect);
	Minion(eCard cardId, eFaction faction, eTribe tribe, int cost, int atk, int hp, std::string path, std::string name, bool isToken, Effect* effect);
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
	void MoveToPosition(int x, int y, bool byEffect);
	void Attack(Minion* target, bool counter);
	int DealDamage(Card* source, int damage);
	int Heal(Card* source, int heal);
	void Destroy(Card* source);
	void Dispel();
	void AddEffects();

	//Utils
	bool CanAttack(Minion* target);
	bool IsMoveable();
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
	int moveRange;
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

	//Updates
	void UpdateStatBuffs();
	void UpdateDetailStats();

	//Action & Event Overrides
	void Resolve(BoardTile* tile);

	//Getter
	Spell* GetSpell() { return this; }

};
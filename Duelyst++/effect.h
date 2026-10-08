#pragma once

//Include
#include <functional>
#include "tile.h"

//Definitions
class Card;
class Effect;
class Game;
class Player;

#pragma region Enums / Helpers

//Keyword flags
enum eKeywordFlags {
	KEYWORD_NONE           = 0,
	KEYWORD_CELERITY       = 1 << 0,
	KEYWORD_FLYING         = 1 << 1,
	KEYWORD_FORCEFIELD     = 1 << 2,
	KEYWORD_OPENING_GAMBIT = 1 << 3,
	KEYWORD_PROVOKE        = 1 << 4,
	KEYWORD_RANGED         = 1 << 5,
	KEYWORD_RUSH           = 1 << 6
};

//Skills and effects
enum eEffect {

	//Effects
	EFFECT_NONE,
	EFFECT_DISPELLED,
	EFFECT_ABJUDICATOR,
	EFFECT_ARAKI_HEADHUNTER,
	EFFECT_ARCHON_SPELLBINDER,
	EFFECT_ARROW_WHISTLER,
	EFFECT_ASTRAL_CRUSADER,
	EFFECT_AZURE_HORN_SHAMAN,
	EFFECT_BASTION,
	EFFECT_CHAKKRAM,
	EFFECT_CONSUMING_REBIRTH,
	EFFECT_CRIMSON_OCULUS,
	EFFECT_DARK_NEMESIS,
	EFFECT_DARKFIRE_SACRIFICE,
	EFFECT_FIRST_SWORD_OF_AKRANE,
	EFFECT_GOLDEN_JUSTICAR,
	EFFECT_GOLEM_METALLURGIST,
	EFFECT_GOLEM_VANQUISHER,
	EFFECT_GROVE_LION,
	EFFECT_LADY_LOCKE,

	//Keyword Skills
	SKILL_CELERITY,
	SKILL_FLYING,
	SKILL_FORCEFIELD,
	SKILL_PROVOKE,
	SKILL_RANGED,
	SKILL_RUSH,

	//Minion Skills
	SKILL_ABJUDICATOR,
	SKILL_AETHERMASTER,
	SKILL_ALCUIN_LOREMASTER,
	SKILL_ALTER_REXX,
	SKILL_ARAKI_HEADHUNTER,
	SKILL_ARCHON_SPELLBINDER,
	SKILL_ARROW_WHISTLER,
	SKILL_ASH_MEPHYT,
	SKILL_ASTRAL_CRUSADER,
	SKILL_AZURE_HERALD,
	SKILL_AZURE_HORN_SHAMAN,
	SKILL_BASTION,
	SKILL_BLACK_LOCUST,
	SKILL_BLAZE_HOUND,
	SKILL_BLISTERING_SKORN,
	SKILL_BLOOD_TAURA,
	SKILL_BLOODTEAR_ALCHEMIST,
	SKILL_BLUETIP_SCORPION,
	SKILL_BONEREAPER,
	SKILL_CAPTAIN_HANK_HART,
	SKILL_CHAKKRAM,
	SKILL_CHAOS_ELEMENTAL,
	SKILL_CRIMSON_OCULUS,
	SKILL_CROSSBONES,
	SKILL_DANCING_BLADES,
	SKILL_DARK_NEMESIS,
	SKILL_DAY_WATCHER,
	SKILL_DEATHBLIGHTER,
	SKILL_DECIMUS,
	SKILL_DIOLTAS,
	SKILL_DREAMGAZER,
	SKILL_DUST_WAILER,
	SKILL_ECLIPSE,
	SKILL_EMERALD_REJUVENATOR,
	SKILL_ENVYBAER,
	SKILL_EPHEMERAL_SHROUD,
	SKILL_EXUN,
	SKILL_FACESTRIKER,
	SKILL_FIRESTARTER,
	SKILL_FIRST_SWORD_OF_AKRANE,
	SKILL_FLAMEBLOOD_WARLOCK,
	SKILL_FROSTBONE_NAGA,
	SKILL_GHOST_LYNX,
	SKILL_GOLDEN_JUSTICAR,
	SKILL_GOLEM_METALLURGIST,
	SKILL_GOLEM_VANQUISHER,
	SKILL_GROVE_LION,
	SKILL_HEALING_MYSTIC,
	SKILL_IRONCLAD,
	SKILL_JAX_TRUESIGHT,
	SKILL_JAXI,
	SKILL_KEEPER_OF_THE_VALE,
	SKILL_KHYMERA,
	SKILL_LADY_LOCKE,
	SKILL_LIGHTBENDER,
	SKILL_LUX_IGNIS,

	//Spells
	SPELL_BREATH_OF_THE_UNBORN,
	SPELL_CONSUMING_REBIRTH,
	SPELL_DAEMONIC_LURE,
	SPELL_DARK_SEED,
	SPELL_DARK_TRANSFORMATION,
	SPELL_DARKFIRE_SACRIFICE

};

//Self reference container for passing to lambdas
class EffectContext {
public:
	EffectContext();
	EffectContext(Effect* effect, Card* card, Game* game);
	~EffectContext();
	Effect* effect;
	Card* card;
	Game* game;
};

#pragma endregion

//Effect class
class Effect {
public:
	Effect();
	Effect(eEffect effect, eKeywordFlags keywords, std::string description);
	~Effect();
	eEffect effect;
	eKeywordFlags keywords;
	std::string description;
	int fixedCost;
	int costBuff;
	int atkBuff;
	int hpBuff;
	Card* token;
	Effect* source;
	void (*OnAddThis)(EffectContext) = nullptr;
	void (*OnRemoveThis)(EffectContext) = nullptr;
	void (*OnPreCastThis)(EffectContext, BoardTile* tile) = nullptr;
	void (*OnResolveThis)(EffectContext, BoardTile* tile) = nullptr;
	void (*OnCast)(EffectContext, Card* card, BoardTile* tile) = nullptr;
	void (*OnSummon)(EffectContext, Minion* minion, bool actionBar) = nullptr;
	void (*OnDeath)(EffectContext, Minion* minion) = nullptr;
	void (*OnAttack)(EffectContext, Minion* source, Minion* target, int& damage, bool counter) = nullptr;
	void (*OnDamage)(EffectContext, Card* source, Minion* target, int damage) = nullptr;
	void (*OnHeal)(EffectContext, Card* source, Minion* target, int damage) = nullptr;
	void (*OnMove)(EffectContext, Minion* minion, bool byEffect) = nullptr;
	void (*OnDraw)(EffectContext, Card* card, bool fromDeck) = nullptr;
	void (*OnReplace)(EffectContext, Card* card) = nullptr;
	void (*OnEffectsChanged)(EffectContext, Card* card) = nullptr;
	void (*OnTurnStart)(EffectContext, Player* player) = nullptr;
	void (*OnTurnEnd)(EffectContext, Player* player) = nullptr;
	bool IsContinuous() { return source != nullptr; }
};
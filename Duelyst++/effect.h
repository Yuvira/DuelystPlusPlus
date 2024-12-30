//Defines
#ifndef __EFFECT_H__
#define __EFFECT_H__

//Include
#include <functional>
#include "tile.h"

//Definitions
class Card;
class Player;

#pragma region Enums

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
	EFFECT_BLOOD_TAURA,
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

	//Skills
	SKILL_CELERITY,
	SKILL_FLYING,
	SKILL_FORCEFIELD,
	SKILL_PROVOKE,
	SKILL_RANGED,
	SKILL_RUSH,
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

#pragma endregion

//Effect class
class Effect {
public:
	Effect();
	Effect(eEffect _effect, eKeywordFlags _keywords, int _costBuff, int _atkBuff, int _hpBuff);
	Effect(eEffect _effect, eKeywordFlags _keywords, int _costBuff, int _atkBuff, int _hpBuff, std::string _description);
	~Effect();
	void GenerateSprite();
	Sprite sprite;
	eEffect effect;
	eKeywordFlags keywords;
	std::string description;
	int costBuff;
	int atkBuff;
	int hpBuff;
	std::vector<Card*> sources;
	std::function<void(Card*, BoardTile*)> OnPreCastThis;
	std::function<void(Card*)> OnDispelThis;
	std::function<void(Card*, Card*, BoardTile*)> OnCast;
	std::function<void(Card*, Minion*, bool)> OnSummon;
	std::function<void(Card*, Minion*)> OnDeath;
	std::function<void(Card*, Player*)> OnTurnEnd;
	std::function<void(Card*, Player*)> OnTurnStart;
	std::string ValueString(int value);
	int TextWidth(std::string str);
	bool IsOpeningGambit();
};

#endif
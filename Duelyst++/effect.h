//Defines
#ifndef __EFFECT_H__
#define __EFFECT_H__

//Include
#include "tile.h"

//Skills
enum eSkill {
	SKILL_NONE,
	SKILL_DISPELLED,
	SKILL_CELERITY,
	SKILL_FLYING,
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
	SKILL_LUX_IGNIS
};

//Effects
enum eEffect {
	EFFECT_NONE,
	EFFECT_DARKFIRE_SACRIFICE,
	EFFECT_GOLDEN_JUSTICAR,
	EFFECT_GOLEM_VANQUISHER,
	EFFECT_GROVE_LION,
	EFFECT_LADY_LOCKE_A,
	EFFECT_LADY_LOCKE_B
};

//Granted stat buffs
enum eBuff {
	BUFF_NONE,
	BUFF_ABJUDICATOR,
	BUFF_ARAKI_HEADHUNTER,
	BUFF_ARCHON_SPELLBINDER,
	BUFF_ARROW_WHISTLER,
	BUFF_ASTRAL_CRUSADER,
	BUFF_AZURE_HORN_SHAMAN,
	BUFF_BASTION,
	BUFF_BLOOD_TAURA,
	BUFF_CHAKKRAM,
	BUFF_CONSUMING_REBIRTH,
	BUFF_CRIMSON_OCULUS,
	BUFF_DARK_NEMESIS,
	BUFF_DARKFIRE_SACRIFICE,
	BUFF_FIRST_SWORD_OF_AKRANE,
	BUFF_GOLEM_METALLURGIST,
	BUFF_LADY_LOCKE
};

//Spell effects
enum eSpell {
	SPELL_NONE,
	SPELL_BREATH_OF_THE_UNBORN,
	SPELL_CONSUMING_REBIRTH,
	SPELL_DAEMONIC_LURE,
	SPELL_DARK_SEED,
	SPELL_DARK_TRANSFORMATION,
	SPELL_DARKFIRE_SACRIFICE
};

//Skill class
class Skill {
public:
	Skill(eSkill = SKILL_NONE);
	~Skill();
	void generateSprite(std::string s);
	bool isOpeningGambit();
	std::vector<Sprite> sprite;
	eSkill skill;
};

//Effect class
class Effect {
public:
	Effect(eEffect = EFFECT_NONE);
	~Effect();
	void generateSprite(std::string s);
	Sprite sprite[2];
	eEffect effect;
	int count;
};

//Stat buff class
class Buff {
public:
	Buff(eBuff = BUFF_NONE, int = 0, int = 0, int = 0, bool = true);
	~Buff();
	void generateSprite(std::string s);
	Sprite sprite;
	eBuff buff;
	int cost;
	int atk;
	int hp;
	bool stacking;
};

//Spell class
class SpellEffect {
public:
	SpellEffect(eSpell = SPELL_NONE);
	~SpellEffect();
	void generateSprite(std::string s);
	std::vector<Sprite> sprite;
	eSpell spell;
};

//Effect list class
class EffectList {
public:
	EffectList();
	~EffectList();
	Skill find(eSkill s);
	Effect find(eEffect e);
	Buff find(eBuff b);
	SpellEffect find(eSpell s);
	std::vector<Skill> skillList;
	std::vector<Effect> effectList;
	std::vector<Buff> buffList;
	std::vector<SpellEffect> spellList;
};

#endif
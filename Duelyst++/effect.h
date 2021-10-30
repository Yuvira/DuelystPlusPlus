//Defines
#ifndef __EFFECT_H__
#define __EFFECT_H__

//Include
#include "tile.h"

//Skills
enum eSkill {
	SKILL_NONE,
	SKILL_FLYING,
	SKILL_AETHERMASTER,
	SKILL_ARAKI_HEADHUNTER,
	SKILL_ASTRAL_CRUSADER,
	SKILL_AZURE_HERALD,
	SKILL_AZURE_HORN_SHAMAN,
	SKILL_BASTION,
	SKILL_BLAZE_HOUND,
	SKILL_BLISTERING_SKORN,
	SKILL_BLOODTEAR_ALCHEMIST,
	SKILL_BLUETIP_SCORPION,
	SKILL_CHAKKRAM,
	SKILL_CHAOS_ELEMENTAL,
	SKILL_GHOST_LYNX
};

//Effects
enum eEffect {
	EFFECT_NONE,
	EFFECT_AETHERMASTER
};

//Granted stat buffs
enum eBuff {
	BUFF_NONE,
	BUFF_ARAKI_HEADHUNTER,
	BUFF_ASTRAL_CRUSADER,
	BUFF_AZURE_HORN_SHAMAN,
	BUFF_BASTION,
	BUFF_CHAKKRAM,
	BUFF_CONSUMING_REBIRTH
};

//Spell effects
enum eSpell {
	SPELL_NONE,
	SPELL_BREATH_OF_THE_UNBORN,
	SPELL_CONSUMING_REBIRTH,
	SPELL_DAEMONIC_LURE,
	SPELL_DARK_SEED,
	SPELL_DARK_TRANSFORMATION
};

//Skill class
class Skill {
public:
	Skill(eSkill = SKILL_NONE);
	~Skill();
	void generateSprite(std::string s);
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
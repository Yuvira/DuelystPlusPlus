//Defines
#ifndef __EFFECT_H__
#define __EFFECT_H__

//Include
#include <vector>
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
	BUFF_BASTION
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
	Buff(eBuff = BUFF_NONE, int = 0, int = 0, int = 0);
	~Buff();
	void generateSprite(std::string s);
	Sprite sprite;
	eBuff buff;
	int cost;
	int atk;
	int hp;
};

//Effect list class
class EffectList {
public:
	EffectList();
	~EffectList();
	Skill find(eSkill s);
	Effect find(eEffect e);
	Buff find(eBuff b);
	std::vector<Skill> slist;
	std::vector<Effect> elist;
	std::vector<Buff> blist;
};

#endif
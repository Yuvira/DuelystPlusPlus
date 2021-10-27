//Defines
#ifndef __EFFECT_H__
#define __EFFECT_H__

//Include
#include <vector>
#include "tile.h"

//Effects
enum eEffect {
	EFFECT_NONE,
	EFFECT_AZURE_HERALD,
	EFFECT_ARAKI_HEADHUNTER,
	EFFECT_GHOST_LYNX
};

//Granted stat buffs
enum eBuff {
	BUFF_NONE,
	BUFF_ARAKI_HEADHUNTER
};

//Effect class
class Effect {
public:
	Effect(eEffect = EFFECT_NONE);
	~Effect();
	void generateSprite(std::string s);
	std::vector<Sprite> sprite;
	eEffect effect;
};

//Stat buff class
class Buff {
public:
	Buff(eBuff = BUFF_NONE, int = 0, int = 0);
	~Buff();
	void generateSprite(std::string s);
	Sprite sprite;
	eBuff buff;
	int atk;
	int hp;
};

//Effect list class
class EffectList {
public:
	EffectList();
	~EffectList();
	Effect find(eEffect e);
	Buff find(eBuff e);
	std::vector<Effect> elist;
	std::vector<Buff> blist;
};

#endif
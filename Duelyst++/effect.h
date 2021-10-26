//Defines
#ifndef __EFFECT_H__
#define __EFFECT_H__

//Include
#include <vector>
#include "tile.h"

//Effects
enum eEffect {
	EFFECT_NONE
};

//Granted stat buffs
enum eBuff {
	BUFF_NONE
};

//Effect class
class Effect {
public:
	Effect();
	~Effect();
	Sprite sprite;
	eEffect effect;
};

//Stat buff class
class Buff {
public:
	Buff();
	~Buff();
	Sprite sprite;
	eBuff buff;
	int atk;
	int hp;
	int count;
};

//Effect list class
class EffectList {
public:
	EffectList();
	~EffectList();
	std::vector<Effect> elist;
	std::vector<Buff> blist;
};

#endif
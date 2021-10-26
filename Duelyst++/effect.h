//Defines
#ifndef __EFFECT_H__
#define __EFFECT_H__

//Include
#include <vector>
#include "tile.h"

//Effects
enum eEffect {
	EFFECT_NONE,
	EFFECT_AZURE_HERALD
};

//Granted stat buffs
enum eBuff {
	BUFF_NONE
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
	Buff(eBuff = BUFF_NONE);
	~Buff();
	Sprite sprite[2];
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
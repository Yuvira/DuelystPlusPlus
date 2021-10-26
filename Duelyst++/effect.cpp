//Include
#include "effect.h"

//Effect constructor/deconstructor
Effect::Effect() { effect = EFFECT_NONE; }
Effect::~Effect() {}

//Buff constructor/deconstructor
Buff::Buff() {
	buff = BUFF_NONE;
	atk = 0;
	hp = 0;
	count = 0;
}
Buff::~Buff() {}

//Effect list constructor/deconstructor
EffectList::EffectList() {}
EffectList::~EffectList() {}
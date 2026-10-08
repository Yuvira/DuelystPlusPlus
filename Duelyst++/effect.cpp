//Include
#include "game.h"

#pragma region Helper Constructors

//Self reference constructor
EffectContext::EffectContext() : EffectContext(nullptr, nullptr, nullptr) {}
EffectContext::EffectContext(Effect* effect, Card* card, Game* game) {
	this->effect = effect;
	this->card = card;
	this->game = game;
}
EffectContext::~EffectContext() {}

#pragma endregion

#pragma region Constructors

//Effect constructors
Effect::Effect() : Effect(EFFECT_NONE, KEYWORD_NONE, "") {}
Effect::Effect(eEffect effect, int keywords, std::string description) {
	this->effect = effect;
	this->keywords = keywords;
	this->description = description;
	triggered = false;
	fixedCost = -1;
	costBuff = 0;
	atkBuff = 0;
	hpBuff = 0;
	moveBuff = 0;
	token = nullptr;
	source = nullptr;
}
Effect::~Effect() {}

#pragma endregion
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
Effect::Effect() : Effect(EFFECT_NONE, KEYWORD_NONE, 0, 0, 0, "") {}
Effect::Effect(eEffect effect, eKeywordFlags keywords, int costBuff, int atkBuff, int hpBuff) : Effect(effect, keywords, costBuff, atkBuff, hpBuff, "") {}
Effect::Effect(eEffect effect, eKeywordFlags keywords, int costBuff, int atkBuff, int hpBuff, std::string description) {
	this->effect = effect;
	this->keywords = keywords;
	this->description = description;
	this->costBuff = costBuff;
	this->atkBuff = atkBuff;
	this->hpBuff = hpBuff;
	source = nullptr;
}
Effect::~Effect() {}

#pragma endregion
//Include
#include "game.h"

#pragma region Helper Constructors

//Self reference constructor
EffectContext::EffectContext() : EffectContext(nullptr, nullptr, nullptr) {}
EffectContext::EffectContext(Effect* _effect, Card* _card, Game* _game) {
	effect = _effect;
	card = _card;
	game = _game;
}
EffectContext::~EffectContext() {}

#pragma endregion

#pragma region Constructors

//Effect constructors
Effect::Effect() : Effect(EFFECT_NONE, KEYWORD_NONE, 0, 0, 0, "") {}
Effect::Effect(eEffect _effect, eKeywordFlags _keywords, int _costBuff, int _atkBuff, int _hpBuff) : Effect(_effect, _keywords, _costBuff, _atkBuff, _hpBuff, "") {}
Effect::Effect(eEffect _effect, eKeywordFlags _keywords, int _costBuff, int _atkBuff, int _hpBuff, std::string _description) {
	effect = _effect;
	keywords = _keywords;
	description = _description;
	costBuff = _costBuff;
	atkBuff = _atkBuff;
	hpBuff = _hpBuff;
	source = nullptr;
}
Effect::~Effect() {}

#pragma endregion
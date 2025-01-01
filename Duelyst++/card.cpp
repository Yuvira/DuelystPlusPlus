//Include
#include "game.h"

#pragma region Helper Constructors

//Targeting mode constructor
TargetMode::TargetMode() : TargetMode(TARGET_MODE_ALL, TARGET_FILTER_NONE) {}
TargetMode::TargetMode(eTargetMode _mode, int _filters) {
	mode = _mode;
	filters = _filters;
}
TargetMode::~TargetMode() {}

#pragma endregion

#pragma region Card Constructor

//Card constructor
Card::Card() {
	faction = FACTION_NEUTRAL;
	cardType = CARD_NONE;
	isToken = false;
	cost = 0;
	game = nullptr;
	owner = nullptr;
	original = this;
	token = nullptr;
	name = "???";
	divider.CreateFromString("컴TOKEN컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴");
}
Card::~Card() {}

#pragma endregion

#pragma region Card Effects

//Add effect to list
void Card::AddEffect(Effect effect, Effect* source) {
	for (int i = 0; i < effects.size(); ++i) {
		if (effects[i].effect == effect.effect) {
			effects[i].sources.push_back(source);
			effects[i].GenerateSprite();
			UpdateStatBuffs();
			return;
		}
	}
	effects.push_back(effect);
	effects.back().sources.push_back(source);
	effects.back().GenerateSprite();
	UpdateStatBuffs();
}

//Remove effect from list
void Card::RemoveEffect(eEffect effect, Effect* source) {
	for (int i = 0; i < effects.size(); ++i) {
		if (effects[i].effect == effect) {
			for (int j = 0; j < effects[i].sources.size(); ++j) {
				if (effects[i].sources[j] == source) {
					effects[i].sources.erase(effects[i].sources.begin() + j);
					--j;
				}
			}
			if (effects[i].sources.size() == 0) {
				effects.erase(effects.begin() + i);
				break;
			}
		}
	}
	UpdateStatBuffs();
}

#pragma endregion

#pragma region Events

//When this is cast (before minion is summoned or spell effects occur)
void Card::PreCast(BoardTile* tile) {
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnPreCastThis)
			effects[i].OnPreCastThis(EffectContext(&effects[i], this, game), tile);
}

//When this resolves (after OnCast events have been sent)
void Card::Resolve(BoardTile* tile) {
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnResolveThis)
			effects[i].OnResolveThis(EffectContext(&effects[i], this, game), tile);
}

//Whenever any card is cast
void Card::OnCast(Card* card, BoardTile* tile) {
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnCast)
			effects[i].OnCast(EffectContext(&effects[i], this, game), card, tile);
}

//Whenever a minion is summoned
void Card::OnSummon(Minion* minion, bool fromActionBar) {
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnSummon)
			effects[i].OnSummon(EffectContext(&effects[i], this, game), minion, fromActionBar);
}

//Whenever a minion dies
void Card::OnDeath(Minion* minion) {
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnDeath)
			effects[i].OnDeath(EffectContext(&effects[i], this, game), minion);
}

//Whenever a minion attacks another minion
void Card::OnAttack(Minion* source, Minion* target, bool counter) {
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnAttack)
			effects[i].OnAttack(EffectContext(&effects[i], this, game), source, target, counter);
}

//Whenever a minion is damaged
void Card::OnDamage(Card* source, Minion* target, int damage) {
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnDamage)
			effects[i].OnDamage(EffectContext(&effects[i], this, game), source, target, damage);
}

//Whenever a minion is healed
void Card::OnHeal(Card* source, Minion* target, int heal) {
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnHeal)
			effects[i].OnHeal(EffectContext(&effects[i], this, game), source, target, heal);
}

//Whenever a minion is moved
void Card::OnMove(Minion* minion, bool byEffect) {
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnMove)
			effects[i].OnMove(EffectContext(&effects[i], this, game), minion, byEffect);
}

//Whenever a card is added to an action bar
void Card::OnDraw(Card* card, bool fromDeck) {
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnDraw)
			effects[i].OnDraw(EffectContext(&effects[i], this, game), card, fromDeck);
}

//Whenever a card is replaced
void Card::OnReplace(Card* replaced) {
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnReplace)
			effects[i].OnReplace(EffectContext(&effects[i], this, game), replaced);
}

//Whenever a player's turn starts
void Card::OnTurnStart(Player* player) {
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnTurnStart)
			effects[i].OnTurnStart(EffectContext(&effects[i], this, game), player);
}

//Whenever a player's turn ends
void Card::OnTurnEnd(Player* player) {
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i].OnTurnEnd)
			effects[i].OnTurnEnd(EffectContext(&effects[i], this, game), player);
}

#pragma endregion
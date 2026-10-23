//Include
#include "carddefinition.h"
#include "card.h"
#include "collections.h"
#include "effect.h"
#include "event.h"
#include "game.h"
#include "renderer.h"
#include "utils.h"

#pragma region Helper Constructors

//Targeting mode constructor
TargetMode::TargetMode() : TargetMode(TARGET_MODE_ALL, nullptr) {}
TargetMode::TargetMode(eTargetMode mode, bool (*Predicate)(BoardTile*)) {
	this->mode = mode;
	this->Predicate = Predicate;
}
TargetMode::~TargetMode() {}

#pragma endregion

#pragma region Card Constructor

//Card constructor
Card::Card(const CardDefinition& definition, Game* game, Player* owner) {
	cardId = definition.cardId;
	rarity = definition.rarity;
	faction = definition.faction;
	name = definition.name;
	baseCost = definition.cost;
	cost = definition.cost;
	this->game = game;
	this->owner = owner;
	if (definition.effect != EFFECT_NONE && game != nullptr)
		AddEffect(definition.effect, this);
	isToken = definition.isToken;
	sprite = definition.sprite;
	detailNameSprite = definition.detailNameSprite;
}
Card::~Card() {}

#pragma endregion

#pragma region Rendering

//Draw sidebar details
void Card::DrawDetails(Renderer& renderer, int& y) {
	if (game == nullptr)
		return;
	sprite.SetColor(COLOR_LTWHITE);
	UpdateDetails();
	game->collections->RenderCardDetails(renderer, 66, y, this);
}

//Update effect details
void Card::UpdateDetails() {

	//Clear
	details.clear();

	//Loop effects
	for (int i = 0; i < effects.size(); ++i) {

		//Add newline if this isn't the first effect in the list
		if (details.size() > 0)
			details.push_back("");

		//Get index of header and add description text
		int headerIdx = details.size();
		Utils::WrapText(effects[i]->ReadDescription(), details, Utils::MAX_DESCRIPTION_WIDTH);

		//Add buffs line if applicable
		effects[i]->ReadModifiers().WriteValues(details);

	}

	//Loop auras
	for (int i = 0; i < auras.size(); ++i) {

		//Add newline if this isn't the first effect in the list
		if (details.size() > 0)
			details.push_back("");

		//Get index of header and add description text
		int headerIdx = details.size();
		Utils::WrapText(auras[i]->ReadDescription(), details, Utils::MAX_DESCRIPTION_WIDTH);

		//Add buffs line if applicable
		auras[i]->ReadModifiers().WriteValues(details);

	}

}

#pragma endregion

#pragma region Stat Updates

//Get stat with modifiers applied
int Card::GetStat(eStat stat, int base, bool withAuras) {

	//If any fixers exist, apply them and ignore everything else
	bool fixed = false;
	for (Effect* effect : effects) {
		if (effect->ReadModifiers().ReadModifier(stat).type == MODTYPE_FIXED) {
			base = effect->ReadModifiers().ReadModifier(stat).value;
			fixed = true;
		}
	}
	if (withAuras) {
		for (Effect* aura : auras) {
			if (aura->ReadModifiers().ReadModifier(stat).type == MODTYPE_FIXED) {
				base = aura->ReadModifiers().ReadModifier(stat).value;
				fixed = true;
			}
		}
	}
	if (fixed)
		return max(base, 0);

	//Apply any rebases
	for (Effect* effect : effects)
		if (effect->ReadModifiers().ReadModifier(stat).type == MODTYPE_REBASE)
			base = effect->ReadModifiers().ReadModifier(stat).value;
	if (withAuras) {
		for (Effect* aura : auras)
			if (aura->ReadModifiers().ReadModifier(stat).type == MODTYPE_REBASE)
				base = aura->ReadModifiers().ReadModifier(stat).value;
	}

	//Apply buffs and overrides in order
	for (Effect* effect : effects) {
		if (effect->ReadModifiers().ReadModifier(stat).type == MODTYPE_BUFF)
			base += effect->ReadModifiers().ReadModifier(stat).value;
		if (effect->ReadModifiers().ReadModifier(stat).type == MODTYPE_OVERRIDE)
			base = effect->ReadModifiers().ReadModifier(stat).value;
	}

	//Auras apply overrides first
	if (withAuras) {
		for (Effect* aura : auras)
			if (aura->ReadModifiers().ReadModifier(stat).type == MODTYPE_OVERRIDE)
				base = aura->ReadModifiers().ReadModifier(stat).value;
		for (Effect* aura : auras)
			if (aura->ReadModifiers().ReadModifier(stat).type == MODTYPE_BUFF)
				base += aura->ReadModifiers().ReadModifier(stat).value;
	}

	//Return modified value
	return max(base, 0);

}

#pragma endregion

#pragma region Card Effects

//Add effect to list, returns pointer to added effect instance
Effect* Card::AddEffect(eEffect effectId, Card* sourceCard) {
	if (effectId == EFFECT_NONE || game == nullptr)
		return nullptr;
	Effect* newEffect = game->collections->GetNewEffectInstance(effectId);
	if (newEffect == nullptr)
		return nullptr;
	effects.push_back(newEffect);
	if (IsOnBoard()) {
		OnAddThisEvent addEvent = OnAddThisEvent();
		newEffect->TriggerSelfEvent(EffectContext(newEffect, this, game), &addEvent);
	}
	UpdateModifiers();
	UpdateAuras();
	return newEffect;
}

//Remove effect from list
void Card::RemoveEffect(Effect* effect) {
	for (int i = 0; i < effects.size(); ++i) {
		if (effects[i] == effect) {
			RemoveEffectAt({i});
			return;
		}
	}
}

//Remove effects at indices
void Card::RemoveEffectAt(std::vector<int> indices) {
	if (indices.empty())
		return;
	std::vector<Effect*> removed;
	for (int i = indices.size() - 1; i >= 0; --i) {
		removed.push_back(effects[indices[i]]);
		effects.erase(effects.begin() + indices[i]);
	}
	for (int i = 0; i < removed.size(); ++i) {
		OnRemoveThisEvent removeEvent = OnRemoveThisEvent();
		removed[i]->TriggerSelfEvent(EffectContext(removed[i], this, game), &removeEvent);
	}
	UpdateModifiers();
	UpdateAuras();
}

//Update aura list
void Card::UpdateAuras() {

	//Return if no game reference to pull auras from
	if (game == nullptr)
		return;

	//Start loop
	bool loop = true;
	while (loop) {

		//Disable flag
		loop = false;

		//Remove untracked auras and those that no longer apply
		std::vector<Aura*> removed;
		for (int i = 0; i < auras.size(); ++i) {
			if (!game->IsTrackingAura(auras[i]) || !auras[i]->MatchesPredicate(game, this)) {
				removed.push_back(auras[i]);
				auras.erase(auras.begin() + i);
				loop = true;
				--i;
			}
		}
		for (Effect* aura : removed) {
			OnRemoveThisEvent removeEvent = OnRemoveThisEvent();
			aura->TriggerSelfEvent(EffectContext(aura, this, game), &removeEvent);
		}

		//Add auras that match predicate
		for (Aura* aura : game->GetAuras()) {
			if (!HasAuraFromSource(aura->GetSourceEffect()) && aura->MatchesPredicate(game, this)) {
				auras.push_back(aura);
				if (IsOnBoard()) {
					OnAddThisEvent addEvent = OnAddThisEvent();
					auras.back()->TriggerSelfEvent(EffectContext(auras.back(), this, game), &addEvent);
				}
				loop = true;
			}
		}

		//Update modifiers
		if (loop)
			UpdateModifiers();

	}

}

//Card is tracking an aura from a given source
bool Card::HasAuraFromSource(Effect* sourceEffect) {
	for (Aura* aura : auras)
		if (aura->GetSourceEffect() == sourceEffect)
			return true;
	return false;
}

#pragma endregion

#pragma region Getters

//Get iterable effect list
std::vector<Effect*> Card::GetEffects() {
	std::vector<Effect*> effectIterator = effects;
	return effectIterator;
}

//Get list of unique tokens included on effects
std::vector<eCard> Card::GetTokens() {
	std::vector<eCard> tokenList;
	for (Effect* effect : effects)
		if (effect->ReadToken() != CARD_NONE && std::find(tokenList.begin(), tokenList.end(), effect->ReadToken()) == tokenList.end())
			tokenList.push_back(effect->ReadToken());
	return tokenList;
}

#pragma endregion

#pragma region Utils

//Get card ownership
bool Card::IsAlly() {
	if (game == nullptr)
		return false;
	return owner == game->GetTurnPlayer();
}
bool Card::IsEnemy() {
	if (game == nullptr)
		return false;
	return owner != game->GetTurnPlayer();
}

#pragma endregion
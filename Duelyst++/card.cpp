//Include
#include "card.h"
#include "game.h"
#include "collections.h"
#include "effect.h"
#include "event.h"
#include "utils.h"

//Constants
const int MAX_DESCRIPTION_WIDTH = 42;

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
Card::Card() {
	cardId = CARD_NONE;
	faction = FACTION_NEUTRAL;
	isToken = false;
	baseCost = 0;
	cost = 0;
	game = nullptr;
	owner = nullptr;
	original = this;
	name = "???";
	divider.CreateFromString("컴TOKEN컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴");
}
Card::~Card() {}

#pragma endregion

#pragma region Rendering

//Update effect details
void Card::UpdateDetails() {

	//Line list
	std::vector<std::string> lines;

	//Loop effects
	for (int i = 0; i < effects.size(); ++i) {

		//Add newline if this isn't the first effect in the list
		if (lines.size() > 0)
			lines.push_back("");

		//Get index of header and add description text
		int headerIdx = lines.size();
		Utils::WrapText(effects[i]->GetDescription(), lines, MAX_DESCRIPTION_WIDTH);

		//Add buffs line if applicable
		effects[i]->GetModifiers().WriteValues(lines);

	}

	//Loop auras
	for (int i = 0; i < auras.size(); ++i) {

		//Add newline if this isn't the first effect in the list
		if (lines.size() > 0)
			lines.push_back("");

		//Get index of header and add description text
		int headerIdx = lines.size();
		Utils::WrapText(auras[i]->GetDescription(), lines, MAX_DESCRIPTION_WIDTH);

		//Add buffs line if applicable
		auras[i]->GetModifiers().WriteValues(lines);

	}

	//Generate
	details.CreateFromTextBlock(lines);

}

#pragma endregion

#pragma region Stat Updates

//Get stat with modifiers applied
int Card::GetStat(eStat stat, int base) {

	//If any fixers exist, apply them and ignore everything else
	bool fixed = false;
	for (Effect* effect : effects) {
		if (effect->GetModifiers().ReadModifier(stat).type == MODTYPE_FIXED) {
			base = effect->GetModifiers().ReadModifier(stat).value;
			fixed = true;
		}
	}
	for (Effect* aura : auras) {
		if (aura->GetModifiers().ReadModifier(stat).type == MODTYPE_FIXED) {
			base = aura->GetModifiers().ReadModifier(stat).value;
			fixed = true;
		}
	}
	if (fixed)
		return max(base, 0);

	//Apply any rebases
	for (Effect* effect : effects)
		if (effect->GetModifiers().ReadModifier(stat).type == MODTYPE_REBASE)
			base = effect->GetModifiers().ReadModifier(stat).value;
	for (Effect* aura : auras)
		if (aura->GetModifiers().ReadModifier(stat).type == MODTYPE_REBASE)
			base = aura->GetModifiers().ReadModifier(stat).value;

	//Apply buffs and overrides in order
	for (Effect* effect : effects) {
		if (effect->GetModifiers().ReadModifier(stat).type == MODTYPE_BUFF)
			base += effect->GetModifiers().ReadModifier(stat).value;
		if (effect->GetModifiers().ReadModifier(stat).type == MODTYPE_OVERRIDE)
			base = effect->GetModifiers().ReadModifier(stat).value;
	}

	//Auras apply overrides first
	for (Effect* aura : auras)
		if (aura->GetModifiers().ReadModifier(stat).type == MODTYPE_OVERRIDE)
			base = aura->GetModifiers().ReadModifier(stat).value;
	for (Effect* aura : auras)
		if (aura->GetModifiers().ReadModifier(stat).type == MODTYPE_BUFF)
			base += aura->GetModifiers().ReadModifier(stat).value;

	//Return modified value
	return max(base, 0);

}

#pragma endregion

#pragma region Card Effects

//Add effect to list
void Card::AddEffect(eEffect effectId, Card* refCard) {
	if (game != nullptr)
		AddEffect(*game->collections->FindEffect(effectId), refCard);
}
void Card::AddEffect(Effect effect, Card* sourceCard) {
	effects.push_back(new Effect(effect));
	effects.back()->SetSources(sourceCard, nullptr);
	if (IsOnBoard()) {
		OnAddThisEvent addEvent = OnAddThisEvent();
		effects.back()->TriggerSelfEvent(EffectContext(effects.back(), this, game), &addEvent);
	}
	UpdateModifiers();
	UpdateAuras();
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
		std::vector<Effect*> removed;
		for (int i = 0; i < auras.size(); ++i) {
			if (!game->IsTrackingAura(auras[i]) || !auras[i]->MatchesAuraPredicate(game, this)) {
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
		for (Effect* aura : game->GetAuras()) {
			if (!HasAuraFromSource(aura->GetSourceEffect()) && aura->MatchesAuraPredicate(game, this)) {
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
	for (Effect* aura : auras)
		if (aura->GetSourceEffect() == sourceEffect)
			return true;
	return false;
}

#pragma endregion

#pragma region Getters

//Getters
std::vector<Effect*> Card::GetEffects() {
	std::vector<Effect*> effectIterator = effects;
	return effectIterator;
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
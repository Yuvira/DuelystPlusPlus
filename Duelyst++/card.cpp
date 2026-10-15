//Include
#include "card.h"
#include "game.h"
#include "collections.h"
#include "effect.h"
#include "event.h"

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
	cost = 0;
	game = nullptr;
	owner = nullptr;
	original = this;
	name = "???";
	divider.CreateFromString("컴TOKEN컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴");
}
Card::~Card() {}

#pragma endregion

#pragma region Updates & Rendering

//Update effect details
void Card::UpdateDetails() {

	//Text lines
	std::vector<std::string> lines;

	//Create pointer list
	std::vector<Effect*> effectRefs;
	for (int i = 0; i < effects.size(); ++i)
		effectRefs.push_back(effects[i]);

	//Loop by removing elements from list until list is empty
	for (; !effectRefs.empty(); effectRefs.erase(effectRefs.begin())) {

		//Get current effect
		Effect* effect = effectRefs.front();

		//Add newline if this isn't the first effect in the list
		if (lines.size() > 0)
			lines.push_back("");

		//Add description to lines
		int headerIdx = lines.size();
		std::string line = "";
		std::string word = "";
		std::string description = effect->GetDescription();
		for (int i = 0; i < description.length(); ++i) {
			if (line.length() == 0 && (description[i] == '|' || description[i] == ' ')) {
				if (description[i] == '|')
					lines.push_back(word);
				else if (description[i] == ' ')
					line += word;
				word = "";
			}
			else if (description[i] == '|') {
				if (TextWidth(line) + TextWidth(word) + 1 > MAX_DESCRIPTION_WIDTH) {
					lines.push_back(line);
					lines.push_back(word);
				}
				else {
					line += ' ' + word;
					lines.push_back(line);
				}
				line = "";
				word = "";
			}
			else if (description[i] == ' ') {
				if (TextWidth(line) + TextWidth(word) + 1 > MAX_DESCRIPTION_WIDTH) {
					lines.push_back(line);
					line = word;
				}
				else
					line += ' ' + word;
				word = "";
			}
			else
				word += description[i];
		}
		if (line.length() == 0)
			lines.push_back(word);
		else if (TextWidth(line) + TextWidth(word) + 1 > MAX_DESCRIPTION_WIDTH) {
			lines.push_back(line);
			lines.push_back(word);
		}
		else
			lines.push_back(line + ' ' + word);

		//Stack count
		int stacks = 1;
		if (effectRefs.size() > 1) {
			for (int i = 1; i < effectRefs.size(); ++i) {
				if (effectRefs[i]->GetID() == effect->GetID()) {
					effectRefs.erase(effectRefs.begin() + i);
					++stacks;
					--i;
				}
			}
			if (stacks > 1 && headerIdx < lines.size())
				lines[headerIdx] += " {[x" + std::to_string(stacks) + "]}";
		}

		//Add buffs line if applicable
		if (effect->GetCostBuff() != 0 || effect->GetAtkBuff() != 0 || effect->GetHPBuff() != 0 || effect->GetMoveBuff() != 0) {
			std::string str = "";
			if (effect->GetAtkBuff() != 0 && effect->GetHPBuff() != 0)
				str += ValueString(effect->GetAtkBuff() * stacks) + "/" + ValueString(effect->GetHPBuff() * stacks);
			else if (effect->GetAtkBuff() != 0)
				str += ValueString(effect->GetAtkBuff() * stacks) + " Attack";
			else if (effect->GetHPBuff() != 0)
				str += ValueString(effect->GetHPBuff() * stacks) + " Health";
			if (effect->GetCostBuff() != 0) {
				if (str != "")
					str += ", ";
				str += ValueString(effect->GetCostBuff() * stacks) + " Cost";
			}
			if (effect->GetMoveBuff() != 0) {
				if (str != "")
					str += ", ";
				str += ValueString(effect->GetMoveBuff() * stacks) + " Movement";
			}
			str = "{" + str + "}";
			lines.push_back(str);
		}

	}

	//Generate
	details.CreateFromTextBlock(lines);

}

#pragma endregion

#pragma region Card Effects

//Add effect to list
void Card::AddEffect(eEffect effectId, Effect* source, Card* refCard) {
	if (game != nullptr)
		AddEffect(*game->collections->FindEffect(effectId), source, refCard);
}
void Card::AddEffect(Effect effect, Effect* source, Card* refCard) {
	if (source != nullptr)
		for (int i = 0; i < effects.size(); ++i)
			if (effects[i]->GetID() == effect.GetID() && effects[i]->GetSource() == source)
				return;
	effects.push_back(new Effect(effect));
	effects.back()->SetSources(source, refCard);
	if (IsOnBoard())
		effects.back()->TriggerOnAddThis(EffectContext(effects.back(), this, game));
	if (game != nullptr) {
		OnEffectsChangedEvent* event = new OnEffectsChangedEvent(game->GetTurnCount(), this);
		game->eventManager.SendOnPreEvent(game, event);
		game->eventManager.SendOnPostEvent(game, event);
		delete event;
	}
	UpdateDetails();
	UpdateStatBuffs();
}

//Add continuous effect to list
void Card::AddContinuousEffect(eEffect effectId, Card* refCard) {
	if (game != nullptr)
		AddContinuousEffect(*game->collections->FindEffect(effectId), refCard);
}
void Card::AddContinuousEffect(Effect effect, Card* refCard) {
	effects.push_back(new Effect(effect));
	effects.back()->SetSources(effects.back(), refCard);
	if (IsOnBoard())
		effects.back()->TriggerOnAddThis(EffectContext(effects.back(), this, game));
	if (game != nullptr) {
		OnEffectsChangedEvent* event = new OnEffectsChangedEvent(game->GetTurnCount(), this);
		game->eventManager.SendOnPreEvent(game, event);
		game->eventManager.SendOnPostEvent(game, event);
		delete event;
	}
	UpdateDetails();
	UpdateStatBuffs();
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

//Remove all effects from a given source
void Card::RemoveEffectsFromSource(Effect* source) {
	std::vector<int> indices;
	for (int i = 0; i < effects.size(); ++i)
		if (effects[i]->GetSource() == source)
			indices.push_back(i);
	RemoveEffectAt(indices);
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
	for (int i = 0; i < removed.size(); ++i)
		removed[i]->TriggerOnRemoveThis(EffectContext(removed[i], this, game));
	OnEffectsChangedEvent* event = new OnEffectsChangedEvent(game->GetTurnCount(), this);
	game->eventManager.SendOnPreEvent(game, event);
	game->eventManager.SendOnPostEvent(game, event);
	delete event;
	UpdateDetails();
	UpdateStatBuffs();
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

//Get value with +/- indicator converted to string
std::string Card::ValueString(int value) { return (value > 0 ? "+" : "-") + std::to_string(abs(value)); }

//Get width of text ignoring style tags
int Card::TextWidth(std::string str) {
	int width = 0;
	for (int i = 0; i < str.length(); ++i)
		if (str[i] != '{' && str[i] != '}')
			++width;
	return width;
}

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
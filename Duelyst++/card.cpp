//Include
#include "game.h"

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
		for (int i = 0; i < effect->description.length(); ++i) {
			if (line.length() == 0 && (effect->description[i] == '|' || effect->description[i] == ' ')) {
				if (effect->description[i] == '|')
					lines.push_back(word);
				else if (effect->description[i] == ' ')
					line += word;
				word = "";
			}
			else if (effect->description[i] == '|') {
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
			else if (effect->description[i] == ' ') {
				if (TextWidth(line) + TextWidth(word) + 1 > MAX_DESCRIPTION_WIDTH) {
					lines.push_back(line);
					line = word;
				}
				else
					line += ' ' + word;
				word = "";
			}
			else
				word += effect->description[i];
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
				if (effectRefs[i]->effect == effect->effect) {
					effectRefs.erase(effectRefs.begin() + i);
					++stacks;
					--i;
				}
			}
			if (stacks > 1 && headerIdx < lines.size())
				lines[headerIdx] += " {[x" + std::to_string(stacks) + "]}";
		}

		//Add buffs line if applicable
		if (effect->costBuff != 0 || effect->atkBuff != 0 || effect->hpBuff != 0 || effect->moveBuff != 0) {
			std::string str = "";
			if (effect->atkBuff != 0 && effect->hpBuff != 0)
				str += ValueString(effect->atkBuff * stacks) + "/" + ValueString(effect->hpBuff * stacks);
			else if (effect->atkBuff != 0)
				str += ValueString(effect->atkBuff * stacks) + " Attack";
			else if (effect->hpBuff != 0)
				str += ValueString(effect->hpBuff * stacks) + " Health";
			if (effect->costBuff != 0) {
				if (str != "")
					str += ", ";
				str += ValueString(effect->costBuff * stacks) + " Cost";
			}
			if (effect->moveBuff != 0) {
				if (str != "")
					str += ", ";
				str += ValueString(effect->moveBuff * stacks) + " Movement";
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
			if (effects[i]->effect == effect.effect && effects[i]->source == source)
				return;
	effects.push_back(new Effect(effect));
	effects.back()->source = source;
	effects.back()->refCard = refCard;
	if (IsOnBoard() && effects.back()->OnAddThis)
		effects.back()->OnAddThis(EffectContext(effects.back(), this, game));
	if (game != nullptr)
		game->eventManager.SendOnEffectsChanged(this);
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
	effects.back()->source = effects.back();
	effects.back()->refCard = refCard;
	if (IsOnBoard() && effects.back()->OnAddThis)
		effects.back()->OnAddThis(EffectContext(effects.back(), this, game));
	if (game != nullptr)
		game->eventManager.SendOnEffectsChanged(this);
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
		if (effects[i]->source == source)
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
		if (removed[i]->OnRemoveThis)
			removed[i]->OnRemoveThis(EffectContext(removed[i], this, game));
	game->eventManager.SendOnEffectsChanged(this);
	UpdateDetails();
	UpdateStatBuffs();
}

#pragma endregion

#pragma region Events

//When this is cast (before minion is summoned or spell effects occur)
void Card::PreCast(BoardTile* tile) {
	std::vector<Effect*> effectIterator = effects;
	for (Effect* effect : effectIterator)
		if (effect->OnPreCastThis)
			effect->OnPreCastThis(EffectContext(effect, this, game), tile);
}

//When this resolves (after OnCast events have been sent)
void Card::Resolve(BoardTile* tile) {
	std::vector<Effect*> effectIterator = effects;
	for (Effect* effect : effectIterator)
		if (effect->OnResolveThis)
			effect->OnResolveThis(EffectContext(effect, this, game), tile);
}

//Whenever any card is cast
void Card::OnCast(Card* card, BoardTile* tile) {
	std::vector<Effect*> effectIterator = effects;
	for (Effect* effect : effectIterator)
		if (effect->OnCast)
			effect->OnCast(EffectContext(effect, this, game), card, tile);
}

//Whenever a minion is summoned
void Card::OnSummon(Minion* minion, bool fromActionBar) {
	std::vector<Effect*> effectIterator = effects;
	for (Effect* effect : effectIterator)
		if (effect->OnSummon)
			effect->OnSummon(EffectContext(effect, this, game), minion, fromActionBar);
}

//Whenever a minion dies
void Card::OnDeath(Minion* minion) {
	std::vector<Effect*> effectIterator = effects;
	for (Effect* effect : effectIterator)
		if (effect->OnDeath)
			effect->OnDeath(EffectContext(effect, this, game), minion);
}

//Whenever a minion attacks another minion
void Card::OnAttack(Minion* source, Minion* target, bool counter) {
	std::vector<Effect*> effectIterator = effects;
	for (Effect* effect : effectIterator)
		if (effect->OnAttack)
			effect->OnAttack(EffectContext(effect, this, game), source, target, counter);
}

//Whenever a minion would be damaged
void Card::OnWouldDealDamage(Card* source, Minion* target, int& damage) {
	std::vector<Effect*> effectIterator = effects;
	for (Effect* effect : effectIterator)
		if (effect->OnWouldDealDamage)
			effect->OnWouldDealDamage(EffectContext(effect, this, game), source, target, damage);
}

//Whenever a minion is damaged
void Card::OnDamageDealt(Card* source, Minion* target, int damage) {
	std::vector<Effect*> effectIterator = effects;
	for (Effect* effect : effectIterator)
		if (effect->OnDamageDealt)
			effect->OnDamageDealt(EffectContext(effect, this, game), source, target, damage);
}

//Whenever a minion would be healed
void Card::OnWouldHeal(Card* source, Minion* target, int& heal) {
	std::vector<Effect*> effectIterator = effects;
	for (Effect* effect : effectIterator)
		if (effect->OnWouldHeal)
			effect->OnWouldHeal(EffectContext(effect, this, game), source, target, heal);
}

//Whenever a minion is healed
void Card::OnHealed(Card* source, Minion* target, int heal) {
	std::vector<Effect*> effectIterator = effects;
	for (Effect* effect : effectIterator)
		if (effect->OnHealed)
			effect->OnHealed(EffectContext(effect, this, game), source, target, heal);
}

//Whenever a minion is moved
void Card::OnMove(Minion* minion, bool byEffect) {
	std::vector<Effect*> effectIterator = effects;
	for (Effect* effect : effectIterator)
		if (effect->OnMove)
			effect->OnMove(EffectContext(effect, this, game), minion, byEffect);
}

//Whenever a card is added to an action bar
void Card::OnDraw(Card* card, bool fromDeck) {
	std::vector<Effect*> effectIterator = effects;
	for (Effect* effect : effectIterator)
		if (effect->OnDraw)
			effect->OnDraw(EffectContext(effect, this, game), card, fromDeck);
}

//Whenever a card is replaced
void Card::OnReplace(Card* replaced, bool& sendToDeck) {
	std::vector<Effect*> effectIterator = effects;
	for (Effect* effect : effectIterator)
		if (effect->OnReplace)
			effect->OnReplace(EffectContext(effect, this, game), replaced, sendToDeck);
}

//Whenever the effects on a card change
void Card::OnEffectsChanged(Card* card) {
	std::vector<Effect*> effectIterator = effects;
	for (Effect* effect : effectIterator)
		if (effect->OnEffectsChanged)
			effect->OnEffectsChanged(EffectContext(effect, this, game), card);
}

//Whenever a player's turn starts
void Card::OnTurnStart(Player* player) {
	std::vector<Effect*> effectIterator = effects;
	for (Effect* effect : effectIterator)
		if (effect->OnTurnStart)
			effect->OnTurnStart(EffectContext(effect, this, game), player);
}

//Whenever a player's turn ends
void Card::OnTurnEnd(Player* player) {
	std::vector<Effect*> effectIterator = effects;
	for (Effect* effect : effectIterator)
		if (effect->OnTurnEnd)
			effect->OnTurnEnd(EffectContext(effect, this, game), player);
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
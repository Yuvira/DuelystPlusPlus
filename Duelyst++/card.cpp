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

#pragma region Updates & Rendering

//Update effect details
void Card::UpdateDetails() {

	//Text lines
	std::vector<std::string> lines;
	lines.push_back("");

	//Create pointer list
	std::vector<Effect*> effectRefs;
	for (int i = 0; i < effects.size(); ++i)
		effectRefs.push_back(&effects[i]);

	//Loop by removing elements from list until list is empty
	for ( ; !effectRefs.empty(); effectRefs.erase(effectRefs.begin())) {

		//Get current effect
		Effect* effect = effectRefs.front();

		//Add newline if this isn't the first effect in the list
		if (lines.size() > 1 || lines[0].length() > 0) {
			lines.push_back("");
			lines.push_back("");
		}

		//Add description to lines
		int headerIdx = lines.size() - 1;
		for (int i = 0; i < effect->description.length(); ++i) {
			if (effect->description[i] == '|')
				lines.push_back("");
			else
				lines.back() += effect->description[i];
		}

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
			if (stacks > 1)
				lines[headerIdx] += " {[x" + std::to_string(stacks) + "]}";
		}

		//Add buffs line if applicable
		if (effect->costBuff != 0 || effect->atkBuff != 0 || effect->hpBuff != 0) {
			std::string str = "";
			if (effect->atkBuff != 0 && effect->hpBuff != 0)
				str += ValueString(effect->atkBuff * stacks) + "/" + ValueString(effect->hpBuff * stacks);
			else if (effect->hpBuff != 0)
				str += ValueString(effect->atkBuff * stacks) + " Attack";
			else if (effect->atkBuff != 0)
				str += ValueString(effect->hpBuff * stacks) + " Health";
			if (effect->costBuff != 0) {
				if (str != "")
					str += ", ";
				str += ValueString(effect->costBuff * stacks) + " Cost";
			}
			str = "{" + str + "}";
			lines.push_back(str);
		}

	}

	//Resize and clear
	int maxWidth = 0;
	for (int i = 0; i < lines.size(); ++i)
		maxWidth = max(maxWidth, TextWidth(lines[i]));
	details.Resize(maxWidth, lines.size());

	//Generate sprite
	int idxDelta = 0;
	WORD color = COLOR_GRAY;
	for (int i = 0; i < lines.size(); ++i) {
		for (int j = 0; j < lines[i].length(); ++j) {
			if (lines[i][j] == '{') {
				color = COLOR_LTWHITE;
				++idxDelta;
			}
			else if (lines[i][j] == '}') {
				color = COLOR_GRAY;
				++idxDelta;
			}
			else {
				details.buffer[(j + (i * details.width)) - idxDelta].Char.AsciiChar = lines[i][j];
				details.buffer[(j + (i * details.width)) - idxDelta].Attributes = color;
			}
		}
		idxDelta = 0;
	}

}

#pragma endregion

#pragma region Card Effects

//Add effect to list
void Card::AddEffect(Effect effect, Effect* source) {
	effects.push_back(effect);
	effects.back().source = source;
	UpdateDetails();
	UpdateStatBuffs();
}

//Remove effect from list
void Card::RemoveEffectsFromSource(Effect* source) {
	for (int i = 0; i < effects.size(); ++i) {
		if (effects[i].source == source) {
			effects.erase(effects.begin() + i);
			--i;
		}
	}
	UpdateDetails();
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

#pragma endregion
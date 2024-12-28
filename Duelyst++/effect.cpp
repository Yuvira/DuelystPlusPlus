//Include
#include "game.h"

#pragma region Constructors

//Effect constructors
Effect::Effect() : Effect(EFFECT_NONE, KEYWORD_NONE, 0, 0, 0, "") {}
Effect::Effect(eEffect _effect, eKeywordFlags _keywords, int _costBuff, int _atkBuff, int _hpBuff) : Effect(_effect, _keywords, _costBuff, _atkBuff, _hpBuff, "") {}
Effect::Effect(eEffect _effect, eKeywordFlags _keywords, int _costBuff, int _atkBuff, int _hpBuff, std::string description) {
	effect = _effect;
	keywords = _keywords;
	costBuff = _costBuff;
	atkBuff = _atkBuff;
	hpBuff = _hpBuff;
	if (description != "")
		GenerateSprite(description);
}
Effect::~Effect() {}

#pragma endregion

#pragma region Rendering

//Generate effect sprite
void Effect::GenerateSprite(std::string str) {

	//Split lines and get max width
	int maxWidth = 0;
	int curWidth = 0;
	std::vector<std::string> lines;
	lines.push_back("");
	for (int i = 0; i < str.length(); ++i) {
		if (str[i] == '|') {
			lines.push_back("");
			if (curWidth > maxWidth)
				maxWidth = curWidth;
			curWidth = 0;
		}
		else {
			lines.back() += str[i];
			if (str[i] != '{' && str[i] != '}')
				++curWidth;
		}
	}

	//Resize and clear
	sprite.Resize(max(maxWidth, curWidth), lines.size());

	//Draw
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
				sprite.buffer[(j + (i * sprite.width)) - idxDelta].Char.AsciiChar = lines[i][j];
				sprite.buffer[(j + (i * sprite.width)) - idxDelta].Attributes = color;
			}
		}
		idxDelta = 0;
	}

	/*
	std::string str = "";
	if (buffs[i].hp == 0 && buffs[i].atk != 0)
		str += (buffs[i].atk > 0 ? "+" : "-") + std::to_string(abs(buffs[i].atk)) + " Attack";
	else if (buffs[i].atk == 0 && buffs[i].hp != 0)
		str += (buffs[i].hp > 0 ? "+" : "-") + std::to_string(abs(buffs[i].hp)) + " Health";
	else if (buffs[i].atk != 0 && buffs[i].hp != 0)
		str += (buffs[i].atk > 0 ? "+" : "-") + std::to_string(buffs[i].atk) + (buffs[i].hp > 0 ? "/+" : "/-") + std::to_string(abs(buffs[i].hp));
	else if (buffs[i].cost == 0)
		str += "+0/+0";
	if (buffs[i].cost != 0) {
		if (str != "")
			str += ", ";
		str += (buffs[i].cost > 0 ? "+" : "-") + std::to_string(abs(buffs[i].cost)) + " Cost";
	}
	*/

}

#pragma endregion

#pragma region Utils

//Is skill an opening gambit
bool Effect::IsOpeningGambit() {
	switch (effect) {
	case SKILL_ABJUDICATOR:
	case SKILL_ALCUIN_LOREMASTER:
	case SKILL_ASH_MEPHYT:
	case SKILL_AZURE_HERALD:
	case SKILL_BLAZE_HOUND:
	case SKILL_BLISTERING_SKORN:
	case SKILL_BLOODTEAR_ALCHEMIST:
	case SKILL_CROSSBONES:
	case SKILL_DANCING_BLADES:
	case SKILL_DEATHBLIGHTER:
	case SKILL_DUST_WAILER:
	case SKILL_EMERALD_REJUVENATOR:
	case SKILL_EPHEMERAL_SHROUD:
	case SKILL_FLAMEBLOOD_WARLOCK:
	case SKILL_FROSTBONE_NAGA:
	case SKILL_GHOST_LYNX:
	case SKILL_HEALING_MYSTIC:
	case SKILL_JAX_TRUESIGHT:
	case SKILL_KEEPER_OF_THE_VALE:
	case SKILL_LADY_LOCKE:
	case SKILL_LIGHTBENDER:
		return true;
		break;
	}
	return false;
}

#pragma endregion
//Include
#include "game.h"

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
}
Effect::~Effect() {}

#pragma endregion

#pragma region Rendering

//Generate effect sprite
void Effect::GenerateSprite() {

	//Split text lines and get max width
	std::vector<std::string> lines;
	lines.push_back("");
	for (int i = 0; i < description.length(); ++i) {
		if (description[i] == '|')
			lines.push_back("");
		else
			lines.back() += description[i];
	}

	//Stack count
	if (sources.size() > 1)
		lines[0] += " {[x" + std::to_string(sources.size()) + "]}";

	//Add buffs line if applicable
	if (costBuff != 0 || atkBuff != 0 || hpBuff != 0) {
		std::string str = "";
		if (atkBuff != 0 && hpBuff != 0)
			str += (atkBuff > 0 ? "+" : "-") + ValueString(atkBuff) + (hpBuff > 0 ? "/+" : "/-") + ValueString(hpBuff);
		else if (hpBuff != 0)
			str += (atkBuff > 0 ? "+" : "-") + ValueString(atkBuff) + " Attack";
		else if (atkBuff != 0)
			str += (hpBuff > 0 ? "+" : "-") + ValueString(hpBuff) + " Health";
		if (costBuff != 0) {
			if (str != "")
				str += ", ";
			str += (costBuff > 0 ? "+" : "-") + ValueString(costBuff) + " Cost";
		}
		str = "{" + str + "}";
		lines.push_back(str);
	}

	//Resize and clear
	int maxWidth = 0;
	for (int i = 0; i < lines.size(); ++i)
		maxWidth = max(maxWidth, TextWidth(lines[i]));
	sprite.Resize(maxWidth, lines.size());

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
				sprite.buffer[(j + (i * sprite.width)) - idxDelta].Char.AsciiChar = lines[i][j];
				sprite.buffer[(j + (i * sprite.width)) - idxDelta].Attributes = color;
			}
		}
		idxDelta = 0;
	}

}

#pragma endregion

#pragma region Utils

//Get absolute value of given int multiplied by stack count converted to string
std::string Effect::ValueString(int value) { return std::to_string(abs(value) * sources.size()); }

//Get width of text ignoring style tags
int Effect::TextWidth(std::string str) {
	int width = 0;
	for (int i = 0; i < str.length(); ++i)
		if (str[i] != '{' && str[i] != '}')
			++width;
	return width;
}

#pragma endregion
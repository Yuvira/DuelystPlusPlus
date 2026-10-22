#pragma once

//Include
#include <algorithm>
#include "sprite.h"

//Namespace
namespace Utils {

#pragma region Internals

	//Local only
	namespace {

		//GenerateStatSprite() helpers
		inline void AddStatToString(std::string& string, std::string& stat, int& index) {
			if (stat.size() > 0) {
				if (string.size() > 0) {
					index = string.size() + 1;
					string += " ";
				}
				string += stat;
			}
		}
		inline void ApplyStatSpriteColor(Sprite& sprite, eColor color, int start, int statLength, int textLength) {
			if (textLength == 0)
				return;
			sprite.SetColor(color, start, start + statLength);
			sprite.SetColor(color, start + statLength + 1, start + textLength + 1 + (textLength - (statLength + 1)));
		}
		inline void GenerateStatSprite(Sprite& sprite, std::string cost, std::string atk, std::string hp) {
			std::string string = cost;
			int costIdx = 0, atkIdx = 0, hpIdx = 0;
			AddStatToString(string, atk, atkIdx);
			AddStatToString(string, hp, hpIdx);
			sprite.CreateFromString(string);
			ApplyStatSpriteColor(sprite, COLOR_LTBLUE, costIdx, 4, cost.size());
			ApplyStatSpriteColor(sprite, COLOR_GREEN, atkIdx, 3, atk.size());
			ApplyStatSpriteColor(sprite, COLOR_RED, hpIdx, 2, hp.size());
		}

	}

#pragma endregion

#pragma region String Utils

	//To uppercase
	inline std::string ToUpper(std::string string) {
		std::string upper;
		upper.resize(string.size());
		std::transform(string.begin(), string.end(), upper.begin(), ::toupper);
		return upper;
	}

	//Get value with +/- indicator converted to string
	inline std::string ValueString(int value) { return (value > 0 ? "+" : "-") + std::to_string(abs(value)); }

	//Get width of text without format markers
	inline int TextWidth(std::string str) {
		int width = 0;
		for (int i = 0; i < str.length(); ++i)
			if (str[i] != '{' && str[i] != '}')
				++width;
		return width;
	}

#pragma region Text Wrapper

	//Description width
	inline constexpr int MAX_DESCRIPTION_WIDTH = 42;

	//Wrap text into string vector around given width
	inline void WrapText(std::string in, std::vector<std::string>& out, int width) {

		//Define current line and word
		std::string line = "";
		std::string word = "";

		//Iterate input string
		for (int i = 0; i < in.length(); ++i) {

			//Reached end of first word in line
			if (line.length() == 0 && (in[i] == '|' || in[i] == ' ')) {

				//Line break, add word to output
				if (in[i] == '|')
					out.push_back(word);

				//Space, add word to line
				else if (in[i] == ' ')
					line += word;

				//Reset current word
				word = "";

			}

			//Line break reached
			else if (in[i] == '|') {

				//Word too long for current line, output separately
				if (TextWidth(line) + TextWidth(word) + 1 > width) {
					out.push_back(line);
					out.push_back(word);
				}

				//Add word to line and output
				else {
					line += ' ' + word;
					out.push_back(line);
				}

				//Reset current
				line = "";
				word = "";

			}

			//Space reached (end of current word)
			else if (in[i] == ' ') {

				//Word too long for current line, output line before adding to next
				if (TextWidth(line) + TextWidth(word) + 1 > width) {
					out.push_back(line);
					line = word;
				}

				//Add word to line
				else
					line += ' ' + word;

				//Reset word
				word = "";
			}

			//Otherwise add character to current word
			else
				word += in[i];

		}

		//Line empty, add last word
		if (line.length() == 0)
			out.push_back(word);

		//Last word too long for current line, output separately
		else if (TextWidth(line) + TextWidth(word) + 1 > width) {
			out.push_back(line);
			out.push_back(word);
		}

		//Otherwise add last word to line before outputting
		else
			out.push_back(line + ' ' + word);

	}

#pragma endregion

#pragma endregion

#pragma region Sprite Utils

	//Get sprite for stat line
	inline void GenerateStatSprite(Sprite& sprite, int cost) { GenerateStatSprite(sprite, "COST:" + std::to_string(cost), "", ""); }
	inline void GenerateStatSprite(Sprite& sprite, int atk, int hp) { GenerateStatSprite(sprite, "", "ATK:" + std::to_string(atk), "HP:" + std::to_string(hp)); }
	inline void GenerateStatSprite(Sprite& sprite, int cost, int atk, int hp) { GenerateStatSprite(sprite, "COST:" + std::to_string(cost), "ATK:" + std::to_string(atk), "HP:" + std::to_string(hp)); }

#pragma endregion

}
#pragma once

//Include
#include <string>
#include <vector>

//Namespace
namespace Utils {

#pragma region String Utils

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

#pragma endregion

#pragma region Text Wrapper

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

}
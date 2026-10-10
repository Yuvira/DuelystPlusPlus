//Include
#include "sprite.h"

#pragma region Constructors

//Sprite constructors
Sprite::Sprite() : Sprite(1, 1) {}
Sprite::Sprite(int width, int height) {
	pos.X = 0;
	pos.Y = 0;
	this->width = width;
	this->height = height;
	for (int i = 0; i < (width * height); ++i) { buffer.push_back(CHAR_INFO()); }
	Clear();
}
Sprite::~Sprite() { }

#pragma endregion

#pragma region Modifiers

//Clear sprites
void Sprite::Clear() {
	for (int i = 0; i < buffer.size(); ++i) {
		buffer[i].Char.AsciiChar = ' ';
		buffer[i].Attributes = COLOR_LTWHITE;
	}
}

//Resize sprites
void Sprite::Resize(int newWidth, int newHeight) {
	width = newWidth;
	height = newHeight;
	buffer.clear();
	for (int i = 0; i < (width * height); ++i)
		buffer.push_back(CHAR_INFO());
	Clear();
}

//Set color
void Sprite::SetColor(eColor color) {
	for (int i = 0; i < buffer.size(); ++i)
		buffer[i].Attributes = color;
}

#pragma endregion

#pragma region Sprite Generators

//Generate sprites from text file
void Sprite::CreateFromFile(std::string filename) {
	std::ifstream file(filename);
	if (file.is_open()) {
		width = 0;
		height = 0;
		std::string in = "";
		for (std::string line; std::getline(file, line); ) {
			in += line; 
			width = line.length();
			++height;
		}
		buffer.clear();
		for (int i = 0; i < (width * height); ++i) {
			buffer.push_back(CHAR_INFO());
			buffer[i].Char.AsciiChar = in[i];
			buffer[i].Attributes = COLOR_LTWHITE;
		}
		file.close();
	}
}

//Generate text sprite from string
void Sprite::CreateFromString(const std::string& str) {
	width = str.length();
	height = 1;
	buffer.clear();
	for (int i = 0; i < str.length(); ++i) {
		buffer.push_back(CHAR_INFO());
		buffer[i].Char.AsciiChar = str[i];
		buffer[i].Attributes = COLOR_LTWHITE;
	}
}

//Generate text sprite from list of strings with color formatting
void Sprite::CreateFromTextBlock(const std::vector<std::string>& lines) {
	int maxWidth = 0;
	for (int i = 0; i < lines.size(); ++i)
		maxWidth = max(maxWidth, TextWidth(lines[i]));
	Resize(maxWidth, lines.size());
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
			else if (lines[i][j] == '`') {
				color = color == COLOR_LTBLUE ? COLOR_GRAY : COLOR_LTBLUE;
				++idxDelta;
			}
			else if (lines[i][j] == '~') {
				color = color == COLOR_LTRED ? COLOR_GRAY : COLOR_LTRED;
				++idxDelta;
			}
			else {
				buffer[(j + (i * width)) - idxDelta].Char.AsciiChar = lines[i][j];
				buffer[(j + (i * width)) - idxDelta].Attributes = color;
			}
		}
		idxDelta = 0;
	}
}

#pragma endregion

#pragma region Utils

//Get width of text ignoring style tags
int Sprite::TextWidth(std::string str) {
	int width = 0;
	for (int i = 0; i < str.length(); ++i)
		if (str[i] != '{' && str[i] != '}' && str[1] != '`' && str[i] != '~')
			++width;
	return width;
}

#pragma endregion
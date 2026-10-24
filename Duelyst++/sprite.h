#pragma once

//Include
#include <string>
#include <vector>
#include "windows.h"

//Color enum
enum eColor {
	COLOR_BLACK = 0,
	COLOR_RED = FOREGROUND_RED,
	COLOR_GREEN = FOREGROUND_GREEN,
	COLOR_BLUE = FOREGROUND_BLUE,
	COLOR_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
	COLOR_PURPLE = FOREGROUND_RED | FOREGROUND_BLUE,
	COLOR_AQUA = FOREGROUND_GREEN | FOREGROUND_BLUE,
	COLOR_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	COLOR_GRAY = FOREGROUND_INTENSITY,
	COLOR_LTRED = FOREGROUND_RED | FOREGROUND_INTENSITY,
	COLOR_LTGREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	COLOR_LTBLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	COLOR_LTYELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	COLOR_LTPURPLE = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	COLOR_LTAQUA = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	COLOR_LTWHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
};

//Sprite class
class Sprite {

public:

	//Constructors
	Sprite();
	Sprite(int width, int height);
	Sprite(char chr);
	Sprite(std::string string);
	~Sprite();

	//Modifiers
	void Clear();
	void Resize(int newWidth, int newHeight);
	void SetColor(eColor color);
	void SetColor(eColor color, int start, int stop);

	//Generators
	void CreateFromFile(std::string filename);
	void CreateFromChar(char chr);
	void CreateFromChar(char chr, eColor color);
	void CreateFromString(const std::string& str);
	void CreateFromString(const std::string& str, eColor color);
	void CreateFromTextBlock(const std::vector<std::string>& lines);

	//Utils
	int GetSize() const { return width * height; }
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	const CHAR_INFO& ReadBuffer(int idx) const {
		if (idx < 0 || idx >= buffer.size())
			return CHAR_INFO();
		return buffer[idx];
	}

private:

	//Properties
	int width, height;
	std::vector<CHAR_INFO> buffer;

};
//Include
#include "sprite.h"

//Sprite constructor / destructor
Sprite::Sprite() : Sprite(1, 1) {}
Sprite::Sprite(int _width, int _height) {
	pos.X = 0;
	pos.Y = 0;
	width = _width;
	height = _height;
	for (int i = 0; i < (width * height); ++i) { buffer.push_back(CHAR_INFO()); }
	Clear();
}
Sprite::~Sprite() { }

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

//Generate text sprites from string
void Sprite::CreateFromString(std::string str) { CreateFromString(str, false); }
void Sprite::CreateFromString(std::string str, bool vertical) {
	if (vertical) {
		width = 1;
		height = str.length();
	}
	else {
		width = str.length();
		height = 1;
	}
	buffer.clear();
	for (int i = 0; i < str.length(); ++i) {
		buffer.push_back(CHAR_INFO());
		buffer[i].Char.AsciiChar = str[i];
		buffer[i].Attributes = COLOR_LTWHITE;
	}
}
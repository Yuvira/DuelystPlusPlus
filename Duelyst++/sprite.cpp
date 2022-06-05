//Include
#include "sprite.h"

//Sprite constructor / destructor
Sprite::Sprite(int sx, int sy) {
	pos.X = 0;
	pos.Y = 0;
	width = sx;
	height = sy;
	for (int a = 0; a < (width * height); ++a) { buffer.push_back(CHAR_INFO()); }
	clear();
}
Sprite::~Sprite() { }

//Clear sprite
void Sprite::clear() {
	for (int a = 0; a < buffer.size(); ++a) {
		buffer[a].Char.AsciiChar = ' ';
		buffer[a].Attributes = COLOR_LTWHITE;
	}
}

//Resize sprite
void Sprite::resize(int sx, int sy) {
	width = sx;
	height = sy;
	buffer.clear();
	for (int a = 0; a < (width * height); ++a) { buffer.push_back(CHAR_INFO()); }
	clear();
}

//Set color
void Sprite::setCol(eColor col) {
	for (int a = 0; a < buffer.size(); ++a) {
		buffer[a].Attributes = col;
	}
}

//Generate sprite from text file
void Sprite::createFromFile(std::string filename) {
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
		for (int a = 0; a < (width * height); ++a) {
			buffer.push_back(CHAR_INFO());
			buffer[a].Char.AsciiChar = in[a];
			buffer[a].Attributes = COLOR_LTWHITE;
		}
		file.close();
	}
}

//Generate text sprite from string
void Sprite::createFromString(std::string s, bool vertical) {
	if (vertical) {
		width = 1;
		height = s.length();
	}
	else {
		width = s.length();
		height = 1;
	}
	buffer.clear();
	for (int a = 0; a < s.length(); ++a) {
		buffer.push_back(CHAR_INFO());
		buffer[a].Char.AsciiChar = s[a];
		buffer[a].Attributes = COLOR_LTWHITE;
	}
}
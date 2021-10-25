//Include
#include "sprite.h"

//Sprite constructor / destructor
Sprite::Sprite(int sx, int sy) {
	pos.X = 0;
	pos.Y = 0;
	aniPos.X = 0;
	aniPos.Y = 0;
	spriteSize.X = sx;
	spriteSize.Y = sy;
	size = sx * sy;
	buffer = new CHAR_INFO[size];
	clear();
}
Sprite::~Sprite() { }

//Clear sprite
void Sprite::clear() {
	for (int a = 0; a < size; ++a) {
		buffer[a].Char.AsciiChar = ' ';
		buffer[a].Attributes = COLOR_LTWHITE;
	}
}

//Resize sprite
void Sprite::resize(int sx, int sy) {
	spriteSize.X = sx;
	spriteSize.Y = sy;
	size = sx * sy;
	buffer = new CHAR_INFO[size];
	clear();
}

//Set color
void Sprite::setCol(eColor col) {
	for (int a = 0; a < size; ++a) {
		buffer[a].Attributes = col;
	}
}

//Generate sprite from text file
void Sprite::createFromFile(std::string filename) {
	std::ifstream file(filename);
	if (file.is_open()) {
		int sx = 0;
		int sy = 0;
		std::string in = "";
		for (std::string line; std::getline(file, line); ) {
			in += line; 
			sx = line.length();
			++sy;
		}
		spriteSize.X = sx;
		spriteSize.Y = sy;
		size = in.length();
		buffer = new CHAR_INFO[size];
		for (int a = 0; a < size; ++a) {
			buffer[a].Char.AsciiChar = in[a];
			buffer[a].Attributes = COLOR_LTWHITE;
		}
		file.close();
	}
}

//Generate text sprite from string
void Sprite::createFromString(std::string s, bool vertical) {
	size = s.length();
	if (vertical) {
		spriteSize.X = 1;
		spriteSize.Y = size;
	}
	else {
		spriteSize.X = size;
		spriteSize.Y = 1;
	}
	buffer = new CHAR_INFO[size];
	for (int a = 0; a < size; ++a) {
		buffer[a].Char.AsciiChar = s[a];
		buffer[a].Attributes = COLOR_LTWHITE;
	}
}
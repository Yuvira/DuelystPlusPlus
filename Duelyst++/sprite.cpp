//Include
#include "sprite.h"

//Sprite constructor
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

//Deconstructor
Sprite::~Sprite() { }

//Clear sprite
void Sprite::clear() {
	for (int a = 0; a < size; ++a) {
		buffer[a].Char.AsciiChar = ' ';
		buffer[a].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
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
			buffer[a].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		}
		file.close();
	}
}
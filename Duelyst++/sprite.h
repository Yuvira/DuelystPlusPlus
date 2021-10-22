//Defines
#ifndef __SPRITE_H__
#define __SPRITE_H__

//Include
#include <string>
#include <fstream>
#include "windows.h"

//Sprite class
class Sprite {
public:
	Sprite(int = 1, int = 1);
	~Sprite();
	void clear();
	void resize(int sx, int sy);
	void createFromFile(std::string filename);
	char symbol;
	int size;
	COORD spriteSize;
	COORD aniPos;
	COORD pos;
	CHAR_INFO * buffer;
};

#endif
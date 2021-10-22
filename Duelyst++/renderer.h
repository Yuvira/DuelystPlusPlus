//Defines
#ifndef __RENDERER_H__
#define __RENDERER_H__

//Include
#include "sprite.h"

//Renderer class
class Renderer {
public:
	Renderer();
	~Renderer();
	void swapBuffer();
	void render(Sprite s);
	void cls();
	void setSize(COORD size);
private:
	HANDLE frameBuffer[2];
	bool currentBuffer;
	COORD startPos;
};

#endif
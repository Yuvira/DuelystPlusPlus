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
	void SetSize(COORD size);
	void SwapBuffer();
	void ClearScreen();
	void Render(Sprite sprite);
	void Render(Sprite sprite, int x, int y);
private:
	HANDLE frameBuffer[2];
	bool currentBuffer;
	COORD startPos;
};

#endif
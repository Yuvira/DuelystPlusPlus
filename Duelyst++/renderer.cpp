//Include
#include "renderer.h"

//Renderer constructor
Renderer::Renderer() {
	frameBuffer[0] = CreateConsoleScreenBuffer (GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	frameBuffer[1] = CreateConsoleScreenBuffer (GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	startPos.X = 0;
	startPos.Y = 0;
	currentBuffer = false;
}

//Deconstructor
Renderer::~Renderer() {}

//Swap screen buffers
void Renderer::swapBuffer() {
	SetConsoleActiveScreenBuffer (frameBuffer[currentBuffer]);
	currentBuffer = !currentBuffer;
}

//Render sprite
void Renderer::render(Sprite s) {
	SMALL_RECT t;
	t.Top = s.pos.Y;
	t.Left = s.pos.X;
	t.Bottom = s.pos.Y + s.spriteSize.Y;
	t.Right = s.pos.X + s.spriteSize.X;
	WriteConsoleOutputA(frameBuffer[currentBuffer], s.buffer, s.spriteSize, s.aniPos, &t);
}

//Clear screen
void Renderer::cls() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD conSize;
	DWORD charsWritten;
	GetConsoleScreenBufferInfo(frameBuffer[currentBuffer], &csbi);
	conSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(frameBuffer[currentBuffer], (TCHAR)' ', conSize, startPos, &charsWritten);
	FillConsoleOutputAttribute(frameBuffer[currentBuffer], csbi.wAttributes, conSize, startPos, &charsWritten);
}

//Set size
void Renderer::setSize(COORD size) {
	SetConsoleScreenBufferSize(frameBuffer[0], size);
	SetConsoleScreenBufferSize(frameBuffer[1], size);
}
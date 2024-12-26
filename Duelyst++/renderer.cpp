//Include
#include "renderer.h"

#pragma region Constructor / Initialization

//Renderer constructor
Renderer::Renderer() {
	frameBuffer[0] = CreateConsoleScreenBuffer (GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	frameBuffer[1] = CreateConsoleScreenBuffer (GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	startPos.X = 0;
	startPos.Y = 0;
	currentBuffer = false;
}
Renderer::~Renderer() {}

//Set size
void Renderer::SetSize(COORD size) {
	SetConsoleScreenBufferSize(frameBuffer[0], size);
	SetConsoleScreenBufferSize(frameBuffer[1], size);
}

#pragma endregion

#pragma region Rendering

//Clear screen
void Renderer::ClearScreen() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD conSize;
	DWORD charsWritten;
	GetConsoleScreenBufferInfo(frameBuffer[currentBuffer], &csbi);
	conSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(frameBuffer[currentBuffer], (TCHAR)' ', conSize, startPos, &charsWritten);
	FillConsoleOutputAttribute(frameBuffer[currentBuffer], csbi.wAttributes, conSize, startPos, &charsWritten);
}

//Swap screen buffers
void Renderer::SwapBuffer() {
	SetConsoleActiveScreenBuffer (frameBuffer[currentBuffer]);
	currentBuffer = !currentBuffer;
}

//Render sprite
void Renderer::Render(Sprite sprite) { Render(sprite, sprite.pos.X, sprite.pos.Y); }
void Renderer::Render(Sprite sprite, int x, int y) {
	SMALL_RECT box;
	box.Top = y;
	box.Left = x;
	box.Bottom = y + sprite.height;
	box.Right = x + sprite.width;
	COORD size;
	size.X = sprite.width;
	size.Y = sprite.height;
	WriteConsoleOutputA(frameBuffer[currentBuffer], &sprite.buffer[0], size, startPos, &box);
}

#pragma endregion
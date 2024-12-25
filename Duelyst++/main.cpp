//Include
#include <ctime>
#include "collection.h"

//Main
int main() {

	//Setup
	system("title Duelyst++");
	system("mode 115,49");
	srand(time(NULL));

	//Variables
	Renderer renderer;
	Collection collection;
	Game game;

	//Switch
	bool doGame = false;
	collection.modeSwitch = &doGame;
	game.modeSwitch = &doGame;

	//Loop
	while (true) {

		//Game
		if (doGame) {
			game.Update();
			renderer.ClearScreen();
			game.RenderGame(renderer);
			renderer.SwapBuffer();
			game.Input();
		}

		//Collection
		else {
			collection.Update();
			renderer.ClearScreen();
			collection.RenderCollection(renderer);
			renderer.SwapBuffer();
			collection.Input();
		}

	}

	//End
	system("pause");
	return 0;

}
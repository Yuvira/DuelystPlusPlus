//Include
#include <ctime>
#include "cardviewer.h"

//Main
int main() {

	//Setup
	system("title Duelyst++");
	system("mode 115,49");
	srand(time(NULL));

	//Variables
	Renderer renderer;
	CardViewer cardViewer;
	Game game;

	//Switch
	bool doGame = false;
	cardViewer.modeSwitch = &doGame;
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
			cardViewer.Update();
			renderer.ClearScreen();
			cardViewer.RenderCollection(renderer);
			renderer.SwapBuffer();
			cardViewer.Input();
		}

	}

	//End
	system("pause");
	return 0;

}
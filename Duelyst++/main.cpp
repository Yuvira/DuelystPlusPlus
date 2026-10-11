//Include
#include <ctime>
#include "cardviewer.h"

//Main
int main() {

	//Setup
	system("title Duelyst++");
	if (IS_DEBUG)
		system("mode 250,49");
	else
		system("mode 115,49");
	srand(time(NULL));

	//Variables
	Renderer renderer;
	Collections collections;
	bool doGame = false;
	CardViewer cardViewer = CardViewer(&collections, &doGame);
	Game game = Game(&collections, &doGame);

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
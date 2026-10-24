//Include
#include <ctime>
#include "game.h"
#include "cardviewer.h"
#include "collections.h"
#include "renderer.h"

//Main
int main() {

	//Setup
	system("title Duelyst++");
	if (IS_DEBUG)
		system("mode 266,49");
	else
		system("mode 115,49");
	srand(time(NULL));

	//Variables
	Renderer renderer;
	Collections collections;
	bool doGame = false;
	Game game = Game(&collections, &doGame);
	CardViewer cardViewer = CardViewer(&game, &collections, &doGame);

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
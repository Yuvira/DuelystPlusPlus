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
	bool modeSwitch = false;
	bool resetGame = false;
	bool exitGame = false;
	Game* game = new Game(&collections, &modeSwitch, &resetGame, &exitGame);
	CardViewer cardViewer = CardViewer(game, &collections, &modeSwitch);

	//Loop
	while (!exitGame) {

		//Game
		if (modeSwitch) {
			game->Update();
			renderer.ClearScreen();
			game->RenderGame(renderer);
			renderer.SwapBuffer();
			game->Input();
		}

		//Collection
		else {
			renderer.ClearScreen();
			cardViewer.RenderCollection(renderer);
			renderer.SwapBuffer();
			cardViewer.Input();
		}

		//Reset game
		if (resetGame) {
			delete game;
			game = new Game(&collections, &modeSwitch, &resetGame, &exitGame);
			cardViewer.SetGame(game);
			resetGame = false;
			modeSwitch = false;
		}

	}

	//End
	delete game;
	system("pause");
	return 0;

}
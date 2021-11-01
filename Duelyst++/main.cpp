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
	Renderer rm;
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
			game.update();
			rm.cls();
			game.render(rm);
			rm.swapBuffer();
			game.input();
		}

		//Collection
		else {
			collection.update();
			rm.cls();
			collection.render(rm);
			rm.swapBuffer();
			collection.input();
		}

	}

	//End
	system("pause");
	return 0;

}
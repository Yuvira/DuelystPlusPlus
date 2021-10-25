//Include
#include <ctime>
#include "game.h"

//Main
int main() {

	//Setup
	system("title Duelyst++");
	system("mode 115,49");
	srand(time(NULL));

	//Variables
	Renderer rm;
	Game game;

	//Game
	while (true) {

		//Update game
		game.update();

		//Render
		rm.cls();
		game.render(rm);
		rm.swapBuffer();

		//Get input
		game.input();

	}

	//End
	system("pause");
	return 0;

}
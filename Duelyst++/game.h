//Defines
#ifndef __GAME_H__
#define __GAME_H__

//Include
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include "eventmanager.h"

//Input mode
enum eMode {
	MODE_NONE,
	MODE_MOVE,
	MODE_HAND,
	MODE_SELECT
};

//Unit effect callback class
class Callback {
public:
	Callback(Unit* = nullptr, Spell* = nullptr, BoardTile* = nullptr, eSkill = SKILL_NONE);
	~Callback();
	Unit* unit;
	Spell* spell;
	BoardTile* tile;
	eSkill skill;
};

//Pathing coords
class PathCoord {
public:
	PathCoord(Coord = Coord(), int = 0, int = 0);
	~PathCoord();
	Coord pos;
	int last;
	int count;
};

//Game class
class Game {
public:
	Game();
	~Game();
	void input();
	void update();
	void render(Renderer& rm);
	void renderSidebar(Renderer& rm);
	void changeTurn(bool t);
	void setContext(Card* c, Player* p);
	void summon(Card* c, int x, int y, bool actionBar);
	void useCard();
	void useEffect();
	void select(BoardTile& t);
	void selectCard();
	void attackUnit();
	void moveUnit();
	void moveCursor(int x, int y);
	void moveCursorHand(int x, int y);
	void moveSelect(int x, int y);
	bool canMove(int x, int y);
	void highlightTile(int x, int y, eColor col);
	void highlightMoveable(int x, int y);
	void highlightSelectable(eTarget type, Unit* = nullptr);
	void createPath();
	bool addToPaths(int x, int y, int l, int c);
	void generatePaths();
	void drawPath(Renderer& rm);
	void drawSword(int x, int y, Renderer& rm);
	void drawArrow(int a, int x, int y, Renderer& rm);
	std::vector<Card*> grave;
	std::vector<Unit*> unit;
	Sprite light;
	Sprite board;
	Sprite chars[8];
	CardList cl;
	EventManager em;
	Player player[2];
	Map map;
	Tile hand[7];
	Callback callback;
	std::vector<Callback> lateCallback;
	std::vector<BoardTile*> highlighted;
	std::vector<BoardTile*> moveable;
	std::vector<BoardTile*> hostile;
	std::vector<BoardTile*> attackable;
	std::vector<BoardTile*> selectable;
	std::vector<PathCoord> pathList;
	std::vector<Coord> path;
	Unit* activeUnit;
	Card* activeCard;
	eMode mode;
	Coord pos;
	int hPos;
	int sPos;
	bool turn;
	bool endTurn;
	int turnCount;
	bool* modeSwitch;
};

#endif
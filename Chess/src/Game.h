#pragma once

#include "Board.h"


enum GameState
{
	GAME_MENU,
	GAME_ON,
	WHITE_WIN,
	BLACK_WIN,
	DRAW
};

class Game
{
public:
	Game();

	void init();
	void play();

private:
	GameState state;
	bool move(Turn& turn, Board& board, Point from, Point to);

	void openVictoryWindow(Color winner);
	void createGameMenu();
};
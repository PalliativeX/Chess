#pragma once

#include "Board.h"


enum GameState
{
	GAME_MENU,
	GAME_ON,
	WHITE_WIN,
	BLACK_WIN,
	DRAW,
	GAME_QUIT
};

class Game
{
public:
	Game();

	void init();
	void play();

private:
	void move(Point from, Point to);

	void openVictoryWindow(Color winner);
	void createGameMenu();
	void render(sf::RenderWindow* mainWindow);
	void processInput(sf::RenderWindow& mainWindow);

private:
	// game state
	Board board;
	GameState gameState;
	Turn turn;
};
#pragma once

#include "ChessPiece.h"

const int BOARD_LENGTH = 8;

class Board {
private:
	ChessPiece * board[BOARD_LENGTH][BOARD_LENGTH];

	bool moveKnight(Point from, Point to);
	bool movePawn(Point from, Point to);
	bool moveRook(Point from, Point to);
	bool moveKing(Point from, Point to);
	bool moveQueen(Point from, Point to);
	bool moveBishop(Point from, Point to);

	bool isCellEmpty();

	void swap(ChessPiece*, ChessPiece*);
	void reverseBoard();

public:
	Board();
	bool move(Point from, Point to, Turn turn);
	void display();
};
#pragma once

#include "ChessPiece.h"

const int BOARD_LENGTH = 8;

class Board {
private:
	ChessPiece * board[BOARD_LENGTH][BOARD_LENGTH];

	// separate move methods for each chess piece
	bool moveKnight(Point from, Point to);
	bool movePawn(Point from, Point to);
	bool moveRook(Point from, Point to);
	bool moveKing(Point from, Point to);
	bool moveQueen(Point from, Point to);
	bool moveBishop(Point from, Point to);

	// @todo probably
	bool isCellEmpty();

	// we move our piece to a chosen square
	void swapAndDeleteSecond(ChessPiece*& to, ChessPiece*& from);

	bool canMoveInLine(Point from, Point to);
	bool canMoveDiagonally(Point from, Point to);
public:
	Board();
	bool move(Point from, Point to, Turn turn);
	void display() const;
};
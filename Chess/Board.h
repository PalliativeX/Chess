#pragma once

#include "ChessPiece.h"

const int BOARD_LENGTH = 8;

class Board {
private:
	ChessPiece * board[BOARD_LENGTH][BOARD_LENGTH];

	// separate move methods for each chess piece, 
	// in each method we also check whether enemy king is under attack
	// if yes, we set isWhite(Black)KingAttacked to true
	bool moveKnight(Point& from, Point& to);
	void attemptToTransformPawn(const Point& to);
	bool movePawn(Point& from, Point& to);
	bool moveRook(Point& from, Point& to);
	bool moveKing(Point& from, Point& to);
	bool moveQueen(Point& from, Point& to);
	bool moveBishop(Point& from, Point& to);


	// we move our piece to a chosen square
	void swapAndDeleteSecond(ChessPiece*& to, ChessPiece*& from);

	
	bool canMoveInLine(const Point& from, const Point& to) const;
	bool canMoveDiagonally(const Point& from, const Point& to) const;
	bool canMoveKing1Square(const Point& from, const Point& to) const; // if it's an enemy piece, we must check whether it is protected
	bool attemptKingCastle(const Point& from, const Point& to, const bool hasKingMoved);

	bool canMoveKnight(const Point& from, const Point& to) const;
	bool canPawnTake(const Point& from, const Point& to) const; // check if a pawn can take either left or right diagonal enemy piece

	const Point getWhiteKingPosition() const;
	const Point getBlackKingPosition() const;
	
public:
	Board();
	bool move(Point& from, Point& to, Turn turn);
	void display() const;

	bool isWhiteKingAttacked = false;
	bool isBlackKingAttacked = false;

	// @todo add saving and loading game, helps in tests
	void saveGame();
	void loadGame();
};
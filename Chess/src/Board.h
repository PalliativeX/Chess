#pragma once

#include "ChessPiece.h"
#include <vector>
#include <algorithm>


const int BOARD_LENGTH = 8;

class Board 
{
private:

	Point prevMove;

	// separate move methods for each chess piece, 
	bool moveKnight(Point& from, Point& to);
	void displayPawnTransformWindow(const Point& to);
	void attemptToTransformPawn(const Point& newPos);
	bool movePawn  (Point& from, Point& to);
	bool moveRook  (Point& from, Point& to);
	bool moveKing  (Point& from, Point& to);
	bool moveQueen (Point& from, Point& to);
	bool moveBishop(Point& from, Point& to);

	// we move our piece to a chosen square
	void swapAndDeleteSecond(ChessPiece*& to, ChessPiece*& from);

	// needed for checking whether a square is protected
	// or a king can go there
	bool canEnemyPiecesReachSquare(const Point& to, const Color currentColor) const;


	bool canMoveInLine(const Point& from, const Point& to) const;
	bool canMoveDiagonally(const Point& from, const Point& to) const;
	bool canMoveKing1Square(const Point& from, const Point& to) const; // if it's an enemy piece, we must check whether it is protected
	bool attemptKingCastle(const Point& from, const Point& to, const bool hasKingMoved);

	bool canMoveKnight(const Point& from, const Point& to) const;
	bool canPawnTake(const Point& from, const Point& to) const; // check if a pawn can take either left or right diagonal enemy piece

	bool canBlockDiagonalAttack(const Point& attackerPos, const Color currentColor) const; // by bishop or queen
	bool canBlockLinearAttack(const Point& attackerPos, const Color currentColor) const; // by rook or queen


public:
	ChessPiece * board[BOARD_LENGTH][BOARD_LENGTH];

	Board();
	bool move(Point& from, Point& to, Turn turn);
	void display() const;


	bool isWhiteKingAttacked = false;
	bool isBlackKingAttacked = false;

	// a king can't be checkmated if it's not even attacked
	bool isWhiteKingCheckmated = false;
	bool isBlackKingCheckmated = false;

	bool isKingCheckmated(Color kingColor);

	const Point getWhiteKingPosition() const;
	const Point getBlackKingPosition() const;

	// we return a previous move, necessary for checking
	// where a piece attacking our king stays
	const Point& getPrevMove() const {
		return prevMove;
	}
	void setPrevMove(Point move) {
		prevMove = move;
	}

	int mainWindowXpos, mainWindowYpos;
};
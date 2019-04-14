#include "Board.h"
#include <iostream>

inline int abs(int x) {
	return (x >= 0 ? x : -x);
}


inline void Board::swapAndDeleteSecond(ChessPiece*& to, ChessPiece*& from) {
	 to = from;
	 from = nullptr;
}


Board::Board() {
	for (int i = 0; i < BOARD_LENGTH; i++) {
		for (int j = 0; j < BOARD_LENGTH; j++) {
			board[i][j] = nullptr;
		}
	}

	// initializing 8 black pawns
	for (int j = 0; j < BOARD_LENGTH; j++) {
		board[1][j] = new ChessPiece(1, PAWN, BLACK);
	}
	//initializing 8 white pawns
	for (int j = 0; j < BOARD_LENGTH; j++) {
		board[6][j] = new ChessPiece(1, PAWN, WHITE);
	}

	// 4 rooks
	board[0][0] = new ChessPiece(4, ROOK, BLACK);
	board[0][7] = new ChessPiece(4, ROOK, BLACK);
	board[7][0] = new ChessPiece(4, ROOK, WHITE);
	board[7][7] = new ChessPiece(4, ROOK, WHITE);

	// 4 knights
	board[0][1] = new ChessPiece(3, KNIGHT, BLACK);
	board[0][6] = new ChessPiece(3, KNIGHT, BLACK);
	board[7][1] = new ChessPiece(3, KNIGHT, WHITE);
	board[7][6] = new ChessPiece(3, KNIGHT, WHITE);

	// 4 bishops
	board[0][2] = new ChessPiece(3, BISHOP, BLACK);
	board[0][5] = new ChessPiece(3, BISHOP, BLACK);
	board[7][2] = new ChessPiece(3, BISHOP, WHITE);
	board[7][5] = new ChessPiece(3, BISHOP, WHITE);

	// 2 queens
	board[0][3] = new ChessPiece(8, QUEEN, BLACK);
	board[7][3] = new ChessPiece(8, QUEEN, WHITE);

	// 2 kings
	board[0][4] = new ChessPiece(50, KING, BLACK);
	board[7][4] = new ChessPiece(50, KING, WHITE);
}

// @todo
bool Board::isCellEmpty() {
	return false;
}

// Color == Turn
bool Board::move(Point from, Point to, Turn turn) {

	// 4 basic checks for validity of move:
	if (from.x > 7 || from.x < 0 ||
		from.y > 7 || from.y < 0)
		return false;

	if (from.x == to.x && from.y == to.y)
		return false;

	if (board[from.x][from.y] == nullptr)
		return false;

	if (board[from.x][from.y]->getColor() != turn)
		return false;

	bool isMoveSuccessful = false;

	// we call a method for a chosen chess piece 
	// @todo can be improved
	ChessPiece* temp = board[from.x][from.y];
	switch (temp->getType()) {
	case PAWN:
		isMoveSuccessful = movePawn(from, to);
		break;
	case KNIGHT:
		isMoveSuccessful = moveKnight(from, to);
		break;
	case BISHOP:
		isMoveSuccessful = moveBishop(from, to);
		break;
	case ROOK:
		isMoveSuccessful = moveRook(from, to);
		break;
	case QUEEN:
		isMoveSuccessful = moveQueen(from, to);
		break;
	case KING:
		isMoveSuccessful = moveKing(from, to);
		break;
	}

	return isMoveSuccessful;
}

// almost correctly, @todo 2-square move
bool Board::movePawn(Point from, Point to) {
	// trying to move pawn 1 square forward
	if (from.y == to.y && (abs(from.x - to.x) == 1)) {
		if (board[to.x][to.y] != nullptr) {
			return false;
		}
		else {
			swapAndDeleteSecond(board[to.x][to.y], board[from.x][from.y]);
			return true;
		}
	}
	// trying to move pawn 2 squares forward
	else if (from.y == to.y && (abs(to.x - from.x) == 2)) {
		if (board[from.x][from.y]->getColor() == WHITE) {
			if ((from.x == 6) && board[to.x][to.y] == nullptr && board[to.x + 1][to.y] == nullptr) {
				swapAndDeleteSecond(board[to.x][to.y], board[from.x][from.y]);
				return true;
			}
		}
		// if piece is BLACK
		else {
			if ((from.x == 1) && board[to.x][to.y] == nullptr && board[to.x - 1][to.y] == nullptr) {
				swapAndDeleteSecond(board[to.x][to.y], board[from.x][from.y]);
				return true;
			}
		}
		return false;
	}
	
	// trying to take the right piece
	else if ((abs(from.x - to.x) == 1) && (abs(to.y - from.y) == 1)) {
		if (board[to.x][to.y] != nullptr) {
			swapAndDeleteSecond(board[to.x][to.y], board[from.x][from.y]);
			return true;
		}
		else return false;
	}

	// trying to take the left piece
	else if ((abs(from.x - to.x) == 1) && (abs(from.y - to.y) == 1)) {
		if (board[to.x][to.y] != nullptr) {
			swapAndDeleteSecond(board[to.x][to.y], board[from.x][from.y]);
			return true;
		}
		else return false;
	}

	return false;
}

// knight works correctly!
bool Board::moveKnight(Point from, Point to) {
	int fromX = from.x;
	int fromY = from.y;
	int toX = to.x;
	int toY = to.y;

	// 2 checks for 8 possible squares
	if (abs(fromX - toX) == 2 && abs(fromY - toY) == 1 ||
		abs(fromX - toX) == 1 && abs(fromY - toY) == 2) 
	{
		swapAndDeleteSecond(board[toX][toY], board[fromX][fromY]);
		return true;
	}

	return false;
}


// can move
bool Board::moveBishop(Point from, Point to) {
	
	bool canMove = canMoveDiagonally(from, to);
	if (canMove) {
		swapAndDeleteSecond(board[to.x][to.y], board[from.x][from.y]);
		return true;
	}


	return false;
}

// works
bool Board::moveRook(Point from, Point to) {

	bool canMove = canMoveInLine(from, to);
	if (canMove) {
		swapAndDeleteSecond(board[to.x][to.y], board[from.x][from.y]);
		return true;
	}
	

	return false;
}

// @todo add castling
bool Board::moveKing(Point from, Point to) {
	for (int i = from.x - 1; i < from.x + 1; i++) {
		for (int j = from.y - 1; j < from.y + 1; j++) {
			if (to.x == i && to.y == j) {
				swapAndDeleteSecond(board[to.x][to.y], board[from.x][from.y]);
				return true;
			}
		}
	}

	return false;
}

// we just call moveRook & moveBishop
// to move our queen vertically, horizontally or diagonally
bool Board::moveQueen(Point from, Point to) {
	if (from.x == to.x && from.y != to.y ||
		from.x != to.x && from.y == to.y) {
		moveRook(from, to);
	}
	else if (abs(from.x - to.x) == abs(from.y - to.y))
		moveBishop(from, to);

	return false;
}

bool Board::canMoveDiagonally(Point from, Point to) {
	// check if an attempt to move is  
	// on diagonal line
	if (abs(from.x - to.x) != abs(from.y - to.y))
		return false;

	// first case, left top branch
	if (from.x > to.x && from.y > to.y) {
		for (int i = from.x - 1, j = from.y - 1; i > to.x, j > to.y; i--, j--) {
			if (board[i][j] != nullptr)
				return false;
		}
		return true;
	}
	// second case, top right branch
	else if (from.x > to.x && from.y < to.y) {
		for (int i = from.x - 1, j = from.y + 1; i > to.x, j < to.y; i--, j++) {
			if (board[i][j] != nullptr)
				return false;
		}
		return true;
	}
	// third case, bottom left branch
	else if (from.x < to.x && from.y > to.y) {
		for (int i = from.x + 1, j = from.y - 1; i < to.x, j > to.y; i++, j--) {
			if (board[i][j] != nullptr)
				return false;
		}
		return true;
	}
	// bottom right branch
	else if (from.x < to.x && from.y < to.y) {
		for (int i = from.x + 1, j = from.y + 1; i < to.x, j < to.y; i++, j++) {
			if (board[i][j] != nullptr)
				return false;
		}
		return true;
	}

	return false;
}

bool Board::canMoveInLine(Point from, Point to) {
	// check if an attempt is really to move on line
	if (!(from.x == to.x && from.y != to.y ||
		from.x != to.x && from.y == to.y))
		return false;

	// top vertical branch
	if (from.x > to.x && from.y == to.y) {
		for (int i = from.x - 1; i > to.x; i--) {
			if (board[i][from.y] != nullptr)
				return false;
		}
		return true;
	}
	// left horizontal branch
	else if (from.x == to.x && from.y > to.y) {
		for (int i = from.y - 1; i > to.y; i--) {
			if (board[from.x][i] != nullptr)
				return false;
		}
		return true;
	}
	// right horizontal branch
	else if (from.x == to.x && from.y < to.y) {
		for (int i = from.y + 1; i < to.y; i++) {
			if (board[from.x][i] != nullptr)
				return false;
		}
		return true;
	}
	// bottom vertical branch
	else if (from.x < to.x && from.y == to.y) {
		for (int i = from.x + 1; i < to.x; i++) {
			if (board[i][from.y] != nullptr)
				return false;
		}
		return true;
	}

	return false;
}


void Board::display() const {
	using namespace std;
	cout << "   y: 0  1  2  3  4  5  6  7 " << endl << "x:" << endl;
	for (int i = 0; i < 8; i++)
	{
		cout << " " << i << "   ";
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] != nullptr) {
				ChessPiece* p = board[i][j];
				Color c = board[i][j]->getColor();

				switch (p->getType())
				{
				case KING: (c == WHITE) ? cout << " K " : cout << " k ";
					break;
				case QUEEN: (c == WHITE) ? cout << " Q " : cout << " q ";
					break;
				case BISHOP:(c == WHITE) ? cout << " B " : cout << " b ";
					break;
				case KNIGHT:(c == WHITE) ? cout << " H " : cout << " h ";
					break;
				case ROOK: (c == WHITE) ? cout << " R " : cout << " r ";
					break;
				case PAWN: (c == WHITE) ? cout << " P " : cout << " p ";
					break;
				default: cout << " . ";
					break;
				}
			}
			else {
				cout << " . ";
			}

		}
		cout << endl;
	}
}
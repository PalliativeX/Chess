#include "Board.h"
#include <iostream>


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


void Board::swap(ChessPiece* a, ChessPiece* b) {
	ChessPiece* temp = a;
	a = b;
	b = temp;
}


// reversing the board so that 
// we can operate with black and white pieces identically
void Board::reverseBoard() {
	for (int i = 0, k = 7; i < 4; i++, k--) {
		for (int j = 0; j < 8; j++) {
			swap(board[i][j], board[k][j]);
		}
	}
}


// Color == Turn
bool Board::move(Point from, Point to, Turn turn) {

	// 3 basic checks for validity of move
	if (from.x > 7 || from.x < 0 ||
		from.y > 7 || from.y < 0)
		return false;

	if (from.x == to.x && from.y == to.y)
		return false;

	if (board[from.x][from.y]->getColor() != turn)
		return false;

	if (turn == BLACK)
		reverseBoard();

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

	if (turn == BLACK)
		reverseBoard();

	return isMoveSuccessful;
}


bool Board::movePawn(Point from, Point to) {
	// trying to move pawn 1 square forward
	if (from.y == to.y && (from.x - to.x == 1)) {
		if (board[to.x][to.y] != nullptr) {
			return false;
		}
		else {
			board[to.x][to.y] = board[from.x][from.y];
			board[from.x][from.y] = nullptr;
			return true;
		}
	}
	// trying to move pawn 2 squares forward
	else if (from.y == to.y && (to.x - from.x == 2)) {
		if (from.x == 6 && board[to.x][to.y] == nullptr && board[to.x + 1][to.y] == nullptr) {
			swap(board[to.x][to.y], board[from.x][from.y]);
			board[from.x][from.y] = nullptr;
			return true;
		}
		else return false;
	}
	
	// trying to take the right piece
	else if ((from.x - to.x == 1) && (to.y - from.y == 1)) {
		if (board[to.x][to.y] != nullptr && board[to.x][to.y]->getColor() != board[from.x][from.y]->getColor()) {
			swap(board[to.x][to.y], board[from.x][from.y]);
			board[from.x][from.y] = nullptr;
			return true;
		}
		else return false;
	}

	// trying to take the left piece
	else if ((from.x - to.x == 1) && (from.y - to.y == 1)) {
		if (board[to.x][to.y] != nullptr && board[to.x][to.y]->getColor() != board[from.x][from.y]->getColor()) {
			swap(board[to.x][to.y], board[from.x][from.y]);
			board[from.x][from.y] = nullptr;
			return true;
		}
		else return false;
	}

	return false;
}


bool Board::moveKnight(Point from, Point to) {
	int fromX = from.x;
	int fromY = from.y;
	int toX = to.x;
	int toY = to.y;
	return false;
}


bool Board::moveBishop(Point from, Point to) {
	return false;
}


bool Board::moveKing(Point from, Point to) {
	return false;
}


bool Board::moveQueen(Point from, Point to) {
	return false;
}


bool Board::moveRook(Point from, Point to) {
	return false;
}

void Board::display() {
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
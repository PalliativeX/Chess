#include "Board.h"
#include <iostream>

inline int abs(int x) {
	return (x >= 0 ? x : -x);
}


inline void Board::swapAndDeleteSecond(ChessPiece*& to, ChessPiece*& from) {
	delete to;
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
		board[1][j] = new ChessPiece(PAWN, BLACK);
	}
	//initializing 8 white pawns
	for (int j = 0; j < BOARD_LENGTH; j++) {
		board[6][j] = new ChessPiece(PAWN, WHITE);
	}

	// 4 rooks
	board[0][0] = new ChessPiece(ROOK, BLACK);
	board[0][7] = new ChessPiece(ROOK, BLACK);
	board[7][0] = new ChessPiece(ROOK, WHITE);
	board[7][7] = new ChessPiece(ROOK, WHITE);

	// 4 knights
	board[0][1] = new ChessPiece(KNIGHT, BLACK);
	board[0][6] = new ChessPiece(KNIGHT, BLACK);
	board[7][1] = new ChessPiece(KNIGHT, WHITE);
	board[7][6] = new ChessPiece(KNIGHT, WHITE);

	// 4 bishops
	board[0][2] = new ChessPiece(BISHOP, BLACK);
	board[0][5] = new ChessPiece(BISHOP, BLACK);
	board[7][2] = new ChessPiece(BISHOP, WHITE);
	board[7][5] = new ChessPiece(BISHOP, WHITE);

	// 2 queens
	board[0][3] = new ChessPiece(QUEEN, BLACK);
	board[7][3] = new ChessPiece(QUEEN, WHITE);

	// 2 kings
	board[0][4] = new ChessPiece(KING, BLACK);
	board[7][4] = new ChessPiece(KING, WHITE);
}


// Color == Turn
bool Board::move(Point& from, Point& to, Turn turn) {

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

void Board::attemptToTransformPawn(const Point& to) {
	// here we check if a pawn reached the end, then
	// we can transform it into another piece
	if (to.x == 0 || to.x == 7) {
		std::cout << " Choose a piece you want your pawn to become: 0 - PAWN, 1 - KNIGHT, 2 - BISHOP, 3 - ROOK, 4 - QUEEN" << std::endl;
		int pieceType;
		while (true) {
			std::cin >> pieceType;
			if (pieceType >= 0 && pieceType <= 4) {
				board[to.x][to.y] = new ChessPiece(static_cast<PieceType>(pieceType), board[to.x][to.y]->getColor());
				break;
			}
		}
	}

}


bool Board::movePawn(Point& from, Point& to) {
	// trying to move pawn 1 square forward
	if (from.y == to.y && (abs(from.x - to.x) == 1)) {
		if (board[to.x][to.y] != nullptr) {
			return false;
		}
		else {
			swapAndDeleteSecond(board[to.x][to.y], board[from.x][from.y]);

			attemptToTransformPawn(to);

			return true;
		}
	}
	// trying to move pawn 2 squares forward
	else if (from.y == to.y && (abs(to.x - from.x) == 2)) {
		// if piece is WHITE
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

	else if (canPawnTake(from, to)) {
		swapAndDeleteSecond(board[to.x][to.y], board[from.x][from.y]);

		attemptToTransformPawn(to);

		// we check whether an enemy king is under attack
		Color currentPieceColor = board[to.x][to.y]->getColor();
		if (canPawnTake(to, (currentPieceColor == WHITE ? getBlackKingPosition() : getWhiteKingPosition()))) {
			currentPieceColor == WHITE ? isBlackKingAttacked = true : isWhiteKingAttacked = true;
		}

		return true;
	}

	return false;
}


// @todo
bool Board::canPawnTake(const Point& from, const Point& to) const {
	// trying to take the right piece
	if ((abs(from.x - to.x) == 1) && (abs(to.y - from.y) == 1)) {
		if (board[to.x][to.y] != nullptr) {
			return true;
		}

		else return false;
	}

	// trying to take the left piece
	else if ((abs(from.x - to.x) == 1) && (abs(from.y - to.y) == 1)) {
		if (board[to.x][to.y] != nullptr) {
			return true;
		}

		else return false;
	}

	return false;
}


bool Board::moveKnight(Point& from, Point& to) {

	if (canMoveKnight(from, to)) {
		swapAndDeleteSecond(board[to.x][to.y], board[from.x][from.y]);

		// here we check if an opposite player's king is attacked
		Color currentPieceColor = board[to.x][to.y]->getColor();
		if (canMoveKnight(to, (currentPieceColor == WHITE ? getBlackKingPosition() : getWhiteKingPosition()))) {
			currentPieceColor == WHITE ? isBlackKingAttacked = true : isWhiteKingAttacked = true;
		}

		return true;
	}

	return false;
}


bool Board::moveBishop(Point& from, Point& to) {

	bool canMove = canMoveDiagonally(from, to);
	if (canMove) {
		swapAndDeleteSecond(board[to.x][to.y], board[from.x][from.y]);

		Color currentPieceColor = board[to.x][to.y]->getColor();
		if (canMoveDiagonally(to, (currentPieceColor == WHITE ? getBlackKingPosition() : getWhiteKingPosition()))) {
			currentPieceColor == WHITE ? isBlackKingAttacked = true : isWhiteKingAttacked = true;
		}

		return true;
	}


	return false;
}


bool Board::moveRook(Point& from, Point& to) {

	bool canMove = canMoveInLine(from, to);
	if (canMove) {
		swapAndDeleteSecond(board[to.x][to.y], board[from.x][from.y]);

		Color currentPieceColor = board[to.x][to.y]->getColor();
		if (canMoveInLine(to, (currentPieceColor == WHITE ? getBlackKingPosition() : getWhiteKingPosition()))) {
			currentPieceColor == WHITE ? isBlackKingAttacked = true : isWhiteKingAttacked = true;
		}

		return true;
	}

	return false;
}

// @todo add castling
bool Board::moveKing(Point& from, Point& to) {
	// these 2 variables need to save info whether king moved or not
	// needed for castling (a king that moved can't castle)
	static bool hasWhiteKingMoved = false;
	static bool hasBlackKingMoved = false;

	if (canMoveKing1Square(from, to))
	{
		board[from.x][from.y]->getColor() == WHITE ? hasWhiteKingMoved = true : hasBlackKingMoved = true;
		swapAndDeleteSecond(board[to.x][to.y], board[from.x][from.y]);
		return true;
	}

	// we pass the value of 1 of static vars to check whether the king with the necessary color
	// moved, which is needed for castling
	if (attemptKingCastle(from, to, (board[from.x][from.y]->getColor() == WHITE ? hasWhiteKingMoved : hasBlackKingMoved))) {
		board[to.x][to.y]->getColor() == WHITE ? hasWhiteKingMoved = true : hasBlackKingMoved = true;
		return true;
	}


	return false;
}

// we just call moveRook & moveBishop
// to move our queen vertically, horizontally or diagonally
bool Board::moveQueen(Point& from, Point& to) {
	if (from.x == to.x && from.y != to.y ||
		from.x != to.x && from.y == to.y) {
		return moveRook(from, to);
	}
	else if (abs(from.x - to.x) == abs(from.y - to.y)) {
		return moveBishop(from, to);
	}

	return false;
}

bool Board::canMoveDiagonally(const Point& from, const Point& to) const {
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

bool Board::canMoveInLine(const Point& from, const Point& to) const {
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


bool Board::canMoveKnight(const Point& from, const Point& to) const {
	// 2 checks for 8 possible squares
	if (abs(from.x - to.x) == 2 && abs(from.y - to.y) == 1 ||
		abs(from.x - to.x) == 1 && abs(from.y - to.y) == 2) 
	{
		return true;
	}

	return false;
}


// @todo - we must check whether a piece is protected (by a queen, e.g.), also if there is a king nearby
// or a square is attacked by some piece
bool Board::canMoveKing1Square(const Point& from, const Point& to) const {
	for (int i = from.x - 1; i < from.x + 1; i++) {
		for (int j = from.y - 1; j < from.y + 1; j++) {
			if (to.x == i && to.y == j) {
				// check if this piece is protected there
				return true;
			}
		}
	}
	return false;
}


bool Board::attemptKingCastle(const Point& from, const Point& to, const bool hasKingMoved) {

	// short castling 4 squares
	if (from.x == to.x && (to.y - from.y == 2) && !hasKingMoved) {
		if (canMoveInLine(from, to)) {
			swapAndDeleteSecond(board[to.x][to.y], board[from.x][from.y]);
			swapAndDeleteSecond(board[to.x][to.y - 1], board[from.x][to.y + 1]);
			return true;
		}

	}

	// long castling 5 squares
	if (from.x == to.x && (from.y - to.y == 2) && !hasKingMoved) {
		if (canMoveInLine(from, to)) {
			swapAndDeleteSecond(board[to.x][to.y], board[from.x][from.y]);
			swapAndDeleteSecond(board[from.x][to.y + 1], board[to.x][to.y - 2]);
			return true;
		}
	}

	return false;
}


// we don't have to return NULL or else, cause the king can't
// be absent on the board
const Point Board::getWhiteKingPosition() const {

	for (int i = 0; i < BOARD_LENGTH; i++) {
		for (int j = 0; j < BOARD_LENGTH; j++) {
			if (board[i][j]->getType() == KING && board[i][j]->getColor() == WHITE)
				return Point(i, j);
		}
	}

	return Point(-1, -1);
}

const Point Board::getBlackKingPosition() const {

	for (int i = 0; i < BOARD_LENGTH; i++) {
		for (int j = 0; j < BOARD_LENGTH; j++) {
			if (board[i][j]->getType() == KING && board[i][j]->getColor() == BLACK)
				return Point(i, j);
		}
	}

	return Point(-1, -1);
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
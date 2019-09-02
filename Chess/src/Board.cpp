#include "Board.h"
#include <iostream>
#include <algorithm>


inline static int absInt(int x) {
	return (x >= 0 ? x : -x);
}


inline void Board::swapAndDeleteSecond(ChessPiece*& to, ChessPiece*& from) {
	delete to;
	to = from;
	from = nullptr;
}


// we check if a square of board equals to nullptr
bool isSquareEmpty(const ChessPiece** p) {
	return (*p != nullptr);
}


// setting up the board and pieces
Board::Board() {
	for (int i = 0; i < BOARD_LENGTH; i++) {
		for (int j = 0; j < BOARD_LENGTH; j++) {
			board[i][j] = nullptr;
		}
	}

	// initializing 8 black pawns
	for (int j = 0; j < BOARD_LENGTH; j++) {
		board[1][j] = new ChessPiece(PAWN, BLACK, 1);
	}
	//initializing 8 white pawns
	for (int j = 0; j < BOARD_LENGTH; j++) {
		board[6][j] = new ChessPiece(PAWN, WHITE, 1);
	}

	// 4 rooks
	board[0][0] = new ChessPiece(ROOK, BLACK, 5.f);
	board[0][7] = new ChessPiece(ROOK, BLACK, 5.f);
	board[7][0] = new ChessPiece(ROOK, WHITE, 5.f);
	board[7][7] = new ChessPiece(ROOK, WHITE, 5.f);

	// 4 knights
	board[0][1] = new ChessPiece(KNIGHT, BLACK, 3.f);
	board[0][6] = new ChessPiece(KNIGHT, BLACK, 3.f);
	board[7][1] = new ChessPiece(KNIGHT, WHITE, 3.f);
	board[7][6] = new ChessPiece(KNIGHT, WHITE, 3.f);

	// 4 bishops
	board[0][2] = new ChessPiece(BISHOP, BLACK, 3.2f);
	board[0][5] = new ChessPiece(BISHOP, BLACK, 3.2f);
	board[7][2] = new ChessPiece(BISHOP, WHITE, 3.2f);
	board[7][5] = new ChessPiece(BISHOP, WHITE, 3.2f);

	// 2 queens
	board[0][3] = new ChessPiece(QUEEN, BLACK, 8.f);
	board[7][3] = new ChessPiece(QUEEN, WHITE, 8.f);

	// 2 kings
	board[0][4] = new ChessPiece(KING, BLACK, 100);
	board[7][4] = new ChessPiece(KING, WHITE, 100);
}


bool isMoveValid(Board& board, Point& from, Point& to, Turn& turn) {
	// 3 basic checks for validity of move:
	if (board.board[from.x][from.y] == nullptr)
		return false;

	if (board.board[from.x][from.y]->getColor() != turn)
		return false;

	if (board.board[to.x][to.y] != nullptr && board.board[to.x][to.y]->getColor() == board.board[from.x][from.y]->getColor())
		return false;

	return true;
}


// Color == Turn
bool Board::move(Point& from, Point& to, Turn turn) {

	if (!isMoveValid(*this, from, to, turn))
		return false;


	bool isMoveSuccessful = false;

	// we call a method for a chosen chess piece 
	ChessPiece* temp = board[from.x][from.y];

	// we have a temp board in case the move is incorrect (exposes the king or doesn't lead to the king leaving the check)
	// so that we can return to the previous state later
	Board tempBoard = *this;

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

	// if the move is wrong we immediately return not to waste time on the following statements
	if (!isMoveSuccessful)
		return isMoveSuccessful;


	// here we check whether a move exposes a king, if so we return to the previous state
	if (canEnemyPiecesReachSquare((turn == WHITE ? getWhiteKingPosition() : getBlackKingPosition()), turn)) {
		*this = tempBoard;
		return false;
	}

	// here, if a king is attacked, we check whether our move changed it,
	// if not, return to the prev. state and return false
	if (turn == WHITE ? isWhiteKingAttacked : isBlackKingAttacked) {
		int attackerX = getPrevMove().x;
		int attackerY = getPrevMove().y;

		if (board[attackerX][attackerY] == nullptr ||
			!canEnemyPiecesReachSquare(turn == WHITE ? getWhiteKingPosition() : getBlackKingPosition(), turn)) {
			(turn == WHITE ? isWhiteKingAttacked : isBlackKingAttacked) = false;
			return isMoveSuccessful;
		}
		else {
			isMoveSuccessful = false;
			*this = tempBoard;
		}
	}

	if (canEnemyPiecesReachSquare(turn == WHITE ? getBlackKingPosition() : getWhiteKingPosition(), turn == WHITE ? BLACK : WHITE)) {
		(turn == WHITE ? isBlackKingAttacked : isWhiteKingAttacked) = true;
	}


	return isMoveSuccessful;
}


void Board::displayPawnTransformWindow(const Point& newPos)
{
	using namespace sf;

	RenderWindow transformWindow(VideoMode(80, 320), "", Style::None);
	transformWindow.setPosition(Vector2i(mainWindowXpos + newPos.y * 80 + 8, mainWindowYpos + newPos.x * 80 + 40));

	static Texture transfWindowTexture;
	static Sprite transfWindowSprite;
	static bool textureInitialized = false;
	if (!textureInitialized) {
		if (board[newPos.x][newPos.y]->getColor() == WHITE)
			transfWindowTexture.loadFromFile("C:/Users/Vladimir/source/repos/Chess/Chess/src/images/transformWindowWhite.png");
		else
			transfWindowTexture.loadFromFile("C:/Users/Vladimir/source/repos/Chess/Chess/src/images/transformWindowBlack.png");
		transfWindowSprite.setTexture(transfWindowTexture);
		textureInitialized = true;
	}

	while (transformWindow.isOpen()) {

		sf::Event event;

		while (transformWindow.pollEvent(event))
		{
			if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
				int xpos = event.mouseButton.x;
				int ypos = event.mouseButton.y;

				// creating a new chess piece depending on where we clicked
				if (ypos < 80)
					board[newPos.x][newPos.y] = new ChessPiece(QUEEN, board[newPos.x][newPos.y]->getColor(), 9);
				else if (ypos < 160)
					board[newPos.x][newPos.y] = new ChessPiece(KNIGHT, board[newPos.x][newPos.y]->getColor(), 3);
				else if (ypos < 240)
					board[newPos.x][newPos.y] = new ChessPiece(ROOK, board[newPos.x][newPos.y]->getColor(), 4);
				else
					board[newPos.x][newPos.y] = new ChessPiece(BISHOP, board[newPos.x][newPos.y]->getColor(), 3);

				transformWindow.close();
			}
		}
		transformWindow.clear();
		transformWindow.draw(transfWindowSprite);
		transformWindow.display();
	}
}


void Board::attemptToTransformPawn(const Point& newPos) {
	// here we check if a pawn reached the end, then
	// we can transform it into another piece
	if (newPos.x == 0 || newPos.x == 7) {
		displayPawnTransformWindow(newPos);
	}
}



bool Board::movePawn(Point& from, Point& to) {
	// trying to move pawn 1 square forward
	if (from.y == to.y && (absInt(from.x - to.x) == 1)) {
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
	else if (from.y == to.y && (absInt(to.x - from.x) == 2)) {
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

		return true;
	}

	return false;
}


bool Board::canPawnTake(const Point& from, const Point& to) const {
	Color color = board[from.x][from.y]->getColor();

	// trying to take the right piece
	if ((absInt(from.x - to.x) == 1) && (absInt(to.y - from.y) == 1)) {
		if (board[to.x][to.y] != nullptr) {
			return true;
		}

		else return false;
	}

	// trying to take the left piece
	else if ((absInt(from.x - to.x) == 1) && (absInt(from.y - to.y) == 1)) {
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

		return true;
	}

	return false;
}


bool Board::moveBishop(Point& from, Point& to) {

	bool canMove = canMoveDiagonally(from, to);
	if (canMove) {

		swapAndDeleteSecond(board[to.x][to.y], board[from.x][from.y]);

		return true;
	}


	return false;
}


bool Board::moveRook(Point& from, Point& to) {

	bool canMove = canMoveInLine(from, to);
	if (canMove) {

		swapAndDeleteSecond(board[to.x][to.y], board[from.x][from.y]);

		return true;
	}

	return false;
}

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
		from.x != to.x && from.y == to.y)
	{
		return moveRook(from, to);
	}
	else if (absInt(from.x - to.x) == absInt(from.y - to.y))
	{
		return moveBishop(from, to);
	}

	return false;
}

bool Board::canMoveDiagonally(const Point& from, const Point& to) const {
	// check if an attempt to move is  
	// on diagonal line
	if (absInt(from.x - to.x) != absInt(from.y - to.y))
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
	if (absInt(from.x - to.x) == 2 && absInt(from.y - to.y) == 1 ||
		absInt(from.x - to.x) == 1 && absInt(from.y - to.y) == 2)
	{
		return true;
	}

	return false;
}


// currentColor is the color of the peace moving right now, its enemies have opposite color
bool Board::canEnemyPiecesReachSquare(const Point& to, const Color currentColor) const {
	for (int i = 0; i < BOARD_LENGTH; i++) {
		for (int j = 0; j < BOARD_LENGTH; j++) {
			// we check if any enemy piece can move to this square
			if (board[i][j] != nullptr && board[i][j]->getColor() != currentColor) {
				switch (board[i][j]->getType()) {
				case PAWN:
					if (canPawnTake(Point(i, j), to))
						return true;
					break;
				case KNIGHT:
					if (canMoveKnight(Point(i, j), to))
						return true;
					break;
				case QUEEN:
					if (canMoveDiagonally(Point(i, j), to))
						return true;
					if (canMoveInLine(Point(i, j), to))
						return true;
					break;
				case ROOK:
					if (canMoveInLine(Point(i, j), to))
						return true;
					break;
				case BISHOP:
					if (canMoveDiagonally(Point(i, j), to))
						return true;
					break;
				}
			}
		}
	}

	return false;
}

// @todo - we must check if there is a king nearby
bool Board::canMoveKing1Square(const Point& from, const Point& to) const {
	if (board[to.x][to.y] != nullptr && board[to.x][to.y]->getColor() == board[from.x][from.y]->getColor())
		return false;

	for (int i = from.x - 1; i <= from.x + 1; i++) {
		for (int j = from.y - 1; j <= from.y + 1; j++) {
			if (to.x == i && to.y == j) {
				if (canEnemyPiecesReachSquare(Point(i, j), board[from.x][from.y]->getColor())) {
					return false;
				}
				return true;
			}
		}
	}
	return false;
}


bool Board::attemptKingCastle(const Point& from, const Point& to, const bool hasKingMoved) {

	Color currColor = board[from.x][from.y]->getColor();

	if ((currColor == WHITE ? isWhiteKingAttacked : isBlackKingAttacked))
		return false;

	// short castling 4 squares
	if (from.x == to.x && (to.y - from.y == 2) && !hasKingMoved) {
		// here we check if squares between the king and rook are not blocked by enemy pieces
		if (canMoveInLine(from, to) &&
			!canEnemyPiecesReachSquare(Point(from.x, from.y + 1), currColor) &&
			!canEnemyPiecesReachSquare(Point(from.x, from.y + 2), currColor))
		{
			swapAndDeleteSecond(board[to.x][to.y], board[from.x][from.y]);
			swapAndDeleteSecond(board[to.x][to.y - 1], board[from.x][to.y + 1]);
			return true;
		}

	}

	// long castling 5 squares
	if (from.x == to.x && (from.y - to.y == 2) && !hasKingMoved) {
		// same check whether squares are blocked
		if (canMoveInLine(from, to) &&
			!canEnemyPiecesReachSquare(Point(from.x, from.y - 1), currColor) &&
			!canEnemyPiecesReachSquare(Point(from.x, from.y - 2), currColor) &&
			!canEnemyPiecesReachSquare(Point(from.x, from.y - 3), currColor))
		{
			swapAndDeleteSecond(board[to.x][to.y], board[from.x][from.y]);
			swapAndDeleteSecond(board[from.x][to.y + 1], board[to.x][to.y - 2]);
			return true;
		}
	}

	return false;
}


// there we must check 4 directions
bool Board::canBlockDiagonalAttack(const Point& attackerPos, const Color currentColor) const {

	Point currentKingPos = (currentColor == WHITE ? getWhiteKingPosition() : getBlackKingPosition());


	// top left branch
	if (currentKingPos.x > attackerPos.x && currentKingPos.y > attackerPos.x) {
		for (int i = attackerPos.x - 1, j = attackerPos.y - 1; i >= currentKingPos.x, j >= currentKingPos.y; i--, j--) {
			if (canEnemyPiecesReachSquare(Point(i, j), (currentColor == WHITE ? BLACK : WHITE))) {
				return true;
			}
		}
	}
	// top right branch
	else if (currentKingPos.x > attackerPos.x && currentKingPos.y < attackerPos.y) {
		for (int i = attackerPos.x - 1, j = attackerPos.y + 1; i <= currentKingPos.x, j <= currentKingPos.y; i--, j++) {
			if (canEnemyPiecesReachSquare(Point(i, j), (currentColor == WHITE ? BLACK : WHITE))) {
				return true;
			}
		}
	}
	// bottom left branch
	else if (currentKingPos.x < attackerPos.x && currentKingPos.y > attackerPos.y) {
		for (int i = attackerPos.x + 1, j = attackerPos.y - 1; i <= currentKingPos.x, j >= currentKingPos.y; i++, j--) {
			if (canEnemyPiecesReachSquare(Point(i, j), (currentColor == WHITE ? BLACK : WHITE))) {
				return true;
			}
		}
	}
	// bottom right branch
	else if (currentKingPos.x < attackerPos.x && currentKingPos.y < attackerPos.y) {
		for (int i = attackerPos.x + 1, j = attackerPos.y + 1; i <= currentKingPos.x, j <= currentKingPos.y; i++, j++) {
			if (canEnemyPiecesReachSquare(Point(i, j), (currentColor == WHITE ? BLACK : WHITE))) {
				return true;
			}
		}
	}


	return false;
}


// same 4 directions
bool Board::canBlockLinearAttack(const Point& attackerPos, const Color currentColor) const {

	Point currentKingPos = (currentColor == WHITE ? getWhiteKingPosition() : getBlackKingPosition());

	// if king is on the same line but higher than attacker
	if (currentKingPos.x < attackerPos.x && currentKingPos.y == attackerPos.y) {
		for (int i = attackerPos.x - 1; i >= currentKingPos.x; i--) {
			if (canEnemyPiecesReachSquare(Point(i, currentKingPos.y), (currentColor == WHITE ? BLACK : WHITE)))
				return true;
		}
	}
	// if king is on the same line but under the attacker
	else if (currentKingPos.x > attackerPos.x && currentKingPos.y == attackerPos.y) {
		for (int i = attackerPos.x + 1; i <= currentKingPos.x; i++) {
			if (canEnemyPiecesReachSquare(Point(i, currentKingPos.y), (currentColor == WHITE ? BLACK : WHITE)))
				return true;
		}
	}
	// if king to the left
	else if (currentKingPos.x == attackerPos.x && currentKingPos.y < attackerPos.y) {
		for (int i = attackerPos.y - 1; i >= currentKingPos.x; i--) {
			if (canEnemyPiecesReachSquare(Point(currentKingPos.x, i), (currentColor == WHITE ? BLACK : WHITE)))
				return true;
		}
	}
	// if king to the right
	else if (currentKingPos.x == attackerPos.x && currentKingPos.y > attackerPos.y) {
		for (int i = attackerPos.y + 1; i <= currentKingPos.x; i++) {
			if (canEnemyPiecesReachSquare(Point(currentKingPos.x, i), (currentColor == WHITE ? BLACK : WHITE)))
				return true;
		}
	}

	return false;
}


// we must check:
// 1 - whether king can escape, 2 - whether a piece which attacks it can be killed, 3 - whether the line can be protected
bool Board::isKingCheckmated(Color kingColor) {
	Point currentKingPos = (kingColor == WHITE ? getWhiteKingPosition() : getBlackKingPosition());

	for (int i = currentKingPos.x - 1; i <= currentKingPos.x + 1; i++) {
		for (int j = currentKingPos.y - 1; j <= currentKingPos.y + 1; j++) {
			if (i < 0 || j < 0 || i > 7 || j > 7)
				continue;

			if (canMoveKing1Square(Point(currentKingPos.x, currentKingPos.y), Point(i, j)))
				return false;
		}
	}

	// we check if our pieces can eliminate this piece, attacking our king
	if (canEnemyPiecesReachSquare(getPrevMove(), kingColor == WHITE ? BLACK : WHITE)) {
		return false;
	}


	ChessPiece* enemyPiece = board[getPrevMove().x][getPrevMove().y];
	// we check here whether the line can be blocked from enemy piece (rook, queen, bishop)
	// we skip this check for knight and pawn, cause they don't attack on line so we can't block 'em
	if (enemyPiece->getType() != KNIGHT && enemyPiece->getType() != PAWN) {

		if (enemyPiece->getType() == ROOK || enemyPiece->getType() == QUEEN) {
			if (canBlockLinearAttack(getPrevMove(), kingColor))
				return false;
		}
		else if (enemyPiece->getType() == QUEEN || enemyPiece->getType() == BISHOP) {
			if (canBlockDiagonalAttack(getPrevMove(), kingColor))
				return false;
		}
	}


	(kingColor == WHITE ? isWhiteKingCheckmated = true : isBlackKingCheckmated = true);
	return true;
}


// we don't have to return NULL or else, cause the king can't
// be absent on the board
const Point Board::getWhiteKingPosition() const {

	for (int i = 0; i < BOARD_LENGTH; i++) {
		for (int j = 0; j < BOARD_LENGTH; j++) {
			if (board[i][j] != nullptr) {
				if (board[i][j]->getType() == KING && board[i][j]->getColor() == WHITE)
					return Point(i, j);
			}
		}
	}

	return Point(-1, -1);
}


const Point Board::getBlackKingPosition() const {

	for (int i = 0; i < BOARD_LENGTH; i++) {
		for (int j = 0; j < BOARD_LENGTH; j++) {
			if (board[i][j] != nullptr) {
				if (board[i][j]->getType() == KING && board[i][j]->getColor() == BLACK)
					return Point(i, j);
			}
		}
	}

	return Point(-1, -1);
}

// only for console output
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
				case KING:   (c == WHITE) ? cout << " K " : cout << " k ";
					break;
				case QUEEN:  (c == WHITE) ? cout << " Q " : cout << " q ";
					break;
				case BISHOP: (c == WHITE) ? cout << " B " : cout << " b ";
					break;
				case KNIGHT: (c == WHITE) ? cout << " H " : cout << " h ";
					break;
				case ROOK:   (c == WHITE) ? cout << " R " : cout << " r ";
					break;
				case PAWN:   (c == WHITE) ? cout << " P " : cout << " p ";
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
#include "Board.h"
#include <iostream>

// main game loop
void play() {
	Board board;

	board.display();

	int turn = WHITE;
	// @todo add win check and draw check
	while (true) {
		using namespace std;

		int xFrom, yFrom, xTo, yTo;
		cout << "Choose the chess piece: " << endl;
		cin >> xFrom >> yFrom;

		cout << "Choose where to move: " << endl;
		cin >> xTo >> yTo;

		Point from(xFrom, yFrom);
		Point to(xTo, yTo);

		if (turn == WHITE) {
			// if a move is successful let BLACK move, else iterate through while() once again
			if (board.move(from, to, WHITE)) {
				board.display();
				if (board.isBlackKingAttacked) {
					std::cout << "Black king is under attack!" << std::endl;
				}
				turn = BLACK;
			}
			else
				cout << "Wrong move!" << endl;
		}
		else {
			if (board.move(from, to, BLACK)) {
				board.display(); 
				if (board.isWhiteKingAttacked) {
					std::cout << "White king is under attack!" << std::endl;
				}
				turn = WHITE;
			}
			else
				cout << "Wrong move" << endl;
		}

	}

}

int main() {

	play();

	return 0;
}
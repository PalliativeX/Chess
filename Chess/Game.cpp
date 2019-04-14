#include "Board.h"
#include <iostream>

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

		if (turn == WHITE) {
			// if a move is successful let BLACK move, else iterate through while() once again
			if (board.move(Point(xFrom, yFrom), Point(xTo, yTo), WHITE)) {
				board.display();
				turn = BLACK;
			}
			else
				cout << "Wrong move!" << endl;
		}
		else {
			if (board.move(Point(xFrom, yFrom), Point(xTo, yTo), BLACK)) {
				board.display();
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
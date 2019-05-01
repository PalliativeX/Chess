#include "Board.h"
#include <iostream>
#include <SFML/Graphics.hpp>

Board newGame() {
	return Board();
}

// main game loop
void play() {
	Board board = newGame();

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
				board.setPrevMove(to);
				if (board.isBlackKingAttacked) {
					if (board.isBlackKingCheckmated) {
						cout << "The black lost, your king is checkmated!" << endl;
						return;
					}
					cout << "Black king is under attack!" << std::endl;
				}
				turn = BLACK;
			}
			else
				cout << "Wrong move!" << endl;
		}
		else {
			if (board.move(from, to, BLACK)) {
				board.display(); 
				board.setPrevMove(to);
				if (board.isWhiteKingAttacked) {
					if (board.isBlackKingCheckmated) {
						cout << "The white lost, your king is checkmated!" << endl;
						return;
					}
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
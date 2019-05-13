#include "Board.h"
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Window/Window.hpp>
#include <string>


Board newGame() {
	return Board();
}

// main game loop
void play() {
	using namespace std;

	
	Board board = newGame();
	/*
	// creating the main window and a board for it
	sf::RenderWindow mainWindow(sf::VideoMode(640, 640), "Chess", sf::Style::Close);

	sf::Texture boardTexture;
	if (!boardTexture.loadFromFile("C:/Users/oxxsana/source/repos/Chess-master/Chess/chess_board.jpg")) {
		std::cout << "Can't load the image!";
	}
	sf::Sprite boardSprite;
	boardSprite.setTexture(boardTexture);

	// loading black and white pieces into Sprite arrays
	sf::Sprite whitePieces[6];
	for (int i = 0; i < 6; i++) {
		sf::Texture texture;
		string fileName = "C:/Users/oxxsana/source/repos/Chess-master/Chess/Pieces/White/" + std::to_string(i) + ".png";
		texture.loadFromFile(fileName);
		whitePieces[i].setTexture(texture);
	}

	sf::Sprite blackPieces[6];
	for (int i = 0; i < 6; i++) {
		sf::Texture texture;
		string fileName = "C:/Users/oxxsana/source/repos/Chess-master/Chess/Pieces/Black/" + std::to_string(i) + ".png";
		texture.loadFromFile(fileName);
		blackPieces[i].setTexture(texture);
	}


	while (mainWindow.isOpen())
	{

		for (int i = 0; i < BOARD_LENGTH; i++) {
			for (int j = 0; j < BOARD_LENGTH; j++) {
				if (board.board[i][j] != nullptr) {
					Color color = board.board[i][j]->getColor();

					switch (board.board[i][j]->getType()) {
					case PAWN:
						(color == WHITE ? whitePieces[0] : blackPieces[0]).setPosition(i * 80 + 20, j * 80 + 20);
						mainWindow.draw(color == WHITE ? whitePieces[0] : blackPieces[0]);
						break;
					case KNIGHT:
						mainWindow.draw(color == WHITE ? whitePieces[1] : blackPieces[1]);
						break;
					case BISHOP:
						mainWindow.draw(color == WHITE ? whitePieces[2] : blackPieces[2]);
						break;
					case ROOK:
						mainWindow.draw(color == WHITE ? whitePieces[3] : blackPieces[3]);
						break;
					case QUEEN:
						mainWindow.draw(color == WHITE ? whitePieces[4] : blackPieces[4]);
						break;
					case KING:
						mainWindow.draw(color == WHITE ? whitePieces[5] : blackPieces[5]);
						break;
					}
				}
			}
		}

		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;

		int turn = WHITE;
		while (mainWindow.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				mainWindow.close();

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				// left mouse button is pressed
				sf::Vector2i currentPos = sf::Mouse::getPosition(mainWindow);
				Point from(currentPos.x / 80, currentPos.y / 80);

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					sf::Vector2i futurePos = sf::Mouse::getPosition(mainWindow);
					Point to(futurePos.x / 80, futurePos.y / 80);

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
		}
		mainWindow.draw(boardSprite);
		mainWindow.display();
	}
}
*/


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
					cout << "Black king is under attack!" << endl;
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
					if (board.isWhiteKingCheckmated) {
						cout << "The white lost, your king is checkmated!" << endl;
						return;
					}
					cout << "White king is under attack!" << endl;
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
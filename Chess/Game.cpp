#include "Board.h"

bool move(Turn& turn, Board& board, Point from, Point to);

// return by value cause we can't return a reference to a local variable
sf::Sprite getNecessarySprite(Color color, PieceType type) {
	static sf::Texture whitePieces[6];
	static sf::Texture blackPieces[6];

	static bool texturesLoaded = false;
	if (!texturesLoaded) {
		// loading textures for white and black pieces only once
		for (int i = 0; i < 6; i++) {
			std::string fileName = "C:/Users/Vladimir/source/repos/Chess/Chess/Images/Pieces/White/" + std::to_string(i) + ".png";
			whitePieces[i].loadFromFile(fileName);
		}

		for (int i = 0; i < 6; i++) {
			std::string fileName = "C:/Users/Vladimir/source/repos/Chess/Chess/Images/Pieces/Black/" + std::to_string(i) + ".png";
			blackPieces[i].loadFromFile(fileName);
		}
		texturesLoaded = true;
	}

	sf::Sprite sprite;
	color == WHITE ? sprite.setTexture(whitePieces[type]) : sprite.setTexture(blackPieces[type]);

	return sprite;
}

// main game loop
void play() {

	Board board;
	
	// creating the main window and a board for it
	sf::RenderWindow mainWindow(sf::VideoMode(640, 640), "Chess", sf::Style::Close);

	sf::Texture boardTexture;
	if (!boardTexture.loadFromFile("C:/Users/Vladimir/source/repos/Chess/Chess/Images/chess_board.jpg")) {
		std::cout << "Can't load the image!";
	}
	sf::Sprite boardSprite;
	boardSprite.setTexture(boardTexture);

	Point from;
	Point to;

	Turn turn = WHITE;

	while (mainWindow.isOpen())
	{
		sf::Vector2i pos = sf::Mouse::getPosition(mainWindow);

		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;

		while (mainWindow.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				mainWindow.close();

			if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 8; j++) {
						if (board.board[i][j] != nullptr && board.board[i][j]->getSprite().getGlobalBounds().contains(pos.x, pos.y)) {
							from.x = i;
							from.y = j;
						}
					}
				}
			}

			if (event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left) {
				to.x = event.mouseButton.y / 80;
				to.y = event.mouseButton.x / 80;

				move(turn, board, from, to);
			}
			
		}

		mainWindow.clear();
		mainWindow.draw(boardSprite);

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (board.board[j][i] != nullptr) {
					board.board[j][i]->getSprite().setPosition(i * 80 + 3, j * 80 + 3);
					mainWindow.draw(board.board[j][i]->getSprite());
				}
			}
		}
		mainWindow.display();
	}
}

bool move(Turn& turn, Board& board, Point from, Point to) {

	if (turn == WHITE) {
		// if a move is successful let BLACK move, else iterate through while() once again
		if (board.move(from, to, WHITE)) {
			board.setPrevMove(to);
			if (board.isBlackKingAttacked) {
				if (board.isBlackKingCheckmated) {
					std::cout << "The black lost, your king is checkmated!" << std::endl;
				}
				std::cout << "Black king is under attack!" << std::endl;
			}
			turn = BLACK;
			return true;
		}
		else {
			return false;
		}

	}
	else {
		if (board.move(from, to, BLACK)) {
			board.setPrevMove(to);
			if (board.isWhiteKingAttacked) {
				if (board.isWhiteKingCheckmated) {
					std::cout << "The white lost, your king is checkmated!" << std::endl;
				}
				std::cout << "White king is under attack!" << std::endl;
			}
			turn = WHITE;
			return true;
		}
		else {
			return false;
		}
	}
}



int main() 
{
	play();

	return 0;
}
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

	// creating the main window
	sf::RenderWindow mainWindow(sf::VideoMode(640, 640), "Chess", sf::Style::Close);

	// creating a game board sprite
	sf::Texture boardTexture;
	boardTexture.loadFromFile("C:/Users/Vladimir/source/repos/Chess/Chess/Images/chess_board.jpg");
	sf::Sprite boardSprite;
	boardSprite.setTexture(boardTexture);

	Point from;
	Point to;

	// here we store color of black and white kings' sprites
	// so that we can restore them later
	Point whiteKingPos = board.getWhiteKingPosition();
	Point blackKingPos = board.getBlackKingPosition();
	sf::Color whiteKingColor = board.board[whiteKingPos.x][whiteKingPos.y]->getSprite().getColor();
	sf::Color blackKingColor = board.board[blackKingPos.x][blackKingPos.y]->getSprite().getColor();

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

		// draw all sprites here
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (board.board[j][i] != nullptr) {
					board.board[j][i]->getSprite().setPosition((float)i * 80 + 5, (float)j * 80 + 3);

					// if a piece is king and it is attacked then we paint it red
					// when a king is not attacked anymore, restore the color
					if (board.board[j][i]->getType() == KING) {
						if (board.board[j][i]->getColor() == WHITE) {
							if (board.isWhiteKingAttacked)
								board.board[j][i]->getSprite().setColor(sf::Color::Red);
							else
								board.board[j][i]->getSprite().setColor(whiteKingColor);
						}


						else if (board.board[j][i]->getColor() == BLACK) {
							if (board.isBlackKingAttacked)
								board.board[j][i]->getSprite().setColor(sf::Color::Red);
							else
								board.board[j][i]->getSprite().setColor(blackKingColor);
						}
					}

					mainWindow.draw(board.board[j][i]->getSprite());
				}
			}
		}
		mainWindow.display();
	}
}

// we pass coords in which we want to move our piece, the turn is changed inside the move method
// if the move was successful
bool move(Turn& turn, Board& board, Point from, Point to) {

	// here we check whether the move is inside the board and
	// the user doesn't try to move a piece to the same position
	if (from.x > 7 || from.x < 0 ||
		from.y > 7 || from.y < 0)
		return false;

	if (to.x > 7 || to.x < 0 ||
		to.y > 7 || to.y < 0)
		return false;

	if (from.x == to.x && from.y == to.y)
		return false;

	if (turn == WHITE) {
		// if a move is successful let BLACK move, else iterate through while() once again
		if (board.move(from, to, WHITE)) {
			board.setPrevMove(to);
			if (board.isBlackKingAttacked) {
				if (board.isKingCheckmated(BLACK)) {
					std::cout << "The black lost, your king is checkmated!" << std::endl;
				}

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
				if (board.isKingCheckmated(WHITE)) {
					std::cout << "The white lost, your king is checkmated!" << std::endl;
				}

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
	using namespace sf;

	RenderWindow menuWindow(VideoMode(500, 400), "Menu", Style::Close);

	// creating a sprite for our menu background
	sf::Texture menuBackgroundTexture;
	menuBackgroundTexture.loadFromFile("C:/Users/Vladimir/source/repos/Chess/Chess/Images/menu_background.jpg");
	sf::Sprite menuBackgroundSprite;
	menuBackgroundSprite.setTexture(menuBackgroundTexture);

	sf::Font font;
	font.loadFromFile("C:/Users/Vladimir/source/repos/Chess/Chess/BakingLion.otf");

	// 3 string for choosing a game mode
	Text pvpOption;
	pvpOption.setFont(font);
	pvpOption.setString("Person vs Person");
	pvpOption.setFillColor(sf::Color::Black);
	pvpOption.setPosition(70, 30);
	pvpOption.setCharacterSize(60);

	Text pvbotOption; 
	pvbotOption.setString("Solo vs Bot");
	pvbotOption.setFillColor(sf::Color::Black);
	pvbotOption.setPosition(110, 140);
	pvbotOption.setFont(font);
	pvbotOption.setCharacterSize(60);

	Text pvpOnlineOption; 
	pvpOnlineOption.setString("Person vs Person Online");
	pvpOnlineOption.setFillColor(sf::Color::Black);
	pvpOnlineOption.setPosition(9, 235);
	pvpOnlineOption.setFont(font);
	pvpOnlineOption.setCharacterSize(60);

	while (menuWindow.isOpen())
	{
		Vector2i pos = sf::Mouse::getPosition(menuWindow);

		Event event;

		while (menuWindow.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				menuWindow.close();

			if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
				if (pvpOption.getGlobalBounds().contains(pos.x, pos.y)) {
					play();
					menuWindow.close();
				}
				if (pvbotOption.getGlobalBounds().contains(pos.x, pos.y)) {

				}
				if (pvpOnlineOption.getGlobalBounds().contains(pos.x, pos.y)) {

				}
			}
		}

		menuWindow.clear();

		menuWindow.draw(menuBackgroundSprite);

		menuWindow.draw(pvpOption);
		menuWindow.draw(pvbotOption);
		menuWindow.draw(pvpOnlineOption);

		menuWindow.display();
	}

	return 0;
}
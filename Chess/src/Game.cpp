#include "Game.h"


Game::Game()
{
}


void Game::init()
{
	gameState = GAME_MENU;
	createGameMenu();
}

// main game loop
void Game::play()
{
	board = Board();

	// creating the main window
	sf::RenderWindow mainWindow(sf::VideoMode(640, 640), "Chess", sf::Style::Close);

	board.mainWindowXpos = mainWindow.getPosition().x;
	board.mainWindowYpos = mainWindow.getPosition().y;

	turn = WHITE;

	while (mainWindow.isOpen())
	{
		// processing input
		processInput(mainWindow);

		// rendering the board
		render(&mainWindow);
	}

}


void Game::processInput(sf::RenderWindow& mainWindow)
{
	static Point from, to;

	sf::Vector2i pos = sf::Mouse::getPosition(mainWindow);

	// check all the window's events that were triggered since the last iteration of the loop
	sf::Event event;

	while (mainWindow.pollEvent(event))
	{
		// "close requested" event: we close the window
		if (event.type == sf::Event::Closed)
			mainWindow.close();

		if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
			for (int i = 0; i < BOARD_LENGTH; i++) {
				for (int j = 0; j < BOARD_LENGTH; j++) {
					if (board.board[i][j] != nullptr && board.board[i][j]->getSprite().getGlobalBounds().contains((float)pos.x, (float)pos.y)) {
						from.x = i;
						from.y = j;
					}
				}
			}
		}

		if (event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left) {
			to.x = event.mouseButton.y / 80;
			to.y = event.mouseButton.x / 80;

			move(from, to);
		}
	}
}

// we pass coords in which we want to move our piece, the turn is changed inside the move method
// if the move was successful
void Game::move(Point from, Point to) {

	// here we check whether the move is inside the board and
	// the user doesn't try to move a piece to the same position
	if (from.isOutOfBoard())
		return;
	if (to.isOutOfBoard())
		return;
	if (from == to)
		return;

	if (turn == WHITE) {
		board.move(from, to, WHITE);
		board.setPrevMove(to);
		if (board.isBlackKingAttacked) {
			if (board.isKingCheckmated(BLACK)) {
				gameState = WHITE_WIN;
			}

		}
		turn = BLACK;
		return;
	}

	else {
		board.move(from, to, BLACK);
		board.setPrevMove(to);
		if (board.isWhiteKingAttacked) {
			if (board.isKingCheckmated(WHITE)) {
				gameState = BLACK_WIN;
			}

		}
		turn = WHITE;
		return;
	}
}


void Game::render(sf::RenderWindow* mainWindow)
{
	// here we store color of black and white kings' sprites
	// so that we can restore them later
	static Point whiteKingPos;
	static Point blackKingPos;
	static sf::Color whiteKingColor;
	static sf::Color blackKingColor;

	// creating a game board sprite
	static sf::Texture boardTexture;
	static sf::Sprite boardSprite;

	static bool initialized;
	if (!initialized) {
		whiteKingPos = board.getWhiteKingPosition();
		blackKingPos = board.getBlackKingPosition();

		whiteKingColor = board.board[whiteKingPos.x][whiteKingPos.y]->getSprite().getColor();
		blackKingColor = board.board[blackKingPos.x][blackKingPos.y]->getSprite().getColor();

		boardTexture.loadFromFile("src/images/chess_board.jpg");
		boardSprite.setTexture(boardTexture);

		initialized = true;
	}


	if (gameState == GAME_ON || gameState == BLACK_WIN || gameState == WHITE_WIN) {

		mainWindow->clear();
		mainWindow->draw(boardSprite);

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

					mainWindow->draw(board.board[j][i]->getSprite());
				}
			}
		}
		mainWindow->display();
	}
	else if (gameState == GAME_MENU) {
		mainWindow->close();
		createGameMenu();
	}

	if (gameState == BLACK_WIN) {
		openVictoryWindow(BLACK);
	}

	if (gameState == WHITE_WIN) {
		openVictoryWindow(WHITE);
	}

	if (gameState == GAME_QUIT) {
		mainWindow->close();
	}
}


void Game::openVictoryWindow(Color winner)
{
	using namespace sf;

	RenderWindow victoryWindow(VideoMode(710, 296), "", Style::None);


	Texture windowTexture;
	windowTexture.loadFromFile("src/images/victory.jpg");
	Sprite windowSprite;
	windowSprite.setTexture(windowTexture);

	sf::Font font;
	font.loadFromFile("src/fonts/MATURASC.TTF");

	Text congrats;
	congrats.setFont(font);
	if (winner == WHITE)
		congrats.setString("The white has won!");
	else
		congrats.setString("The black has won!");
	congrats.setFillColor(sf::Color::Yellow);
	congrats.setPosition(110, 40);
	congrats.setCharacterSize(60);

	Text options;
	options.setFont(font);
	options.setString("Press ENTER to start a new game or ESCAPE to quit");
	options.setFillColor(sf::Color::Yellow);
	options.setPosition(15, 160);
	options.setCharacterSize(25);

	while (victoryWindow.isOpen())
	{
		Event event;

		while (victoryWindow.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
				gameState = GAME_QUIT;
				victoryWindow.close();
			}

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
				gameState = GAME_MENU;
				victoryWindow.close();
			}
		}

		victoryWindow.clear();

		victoryWindow.draw(windowSprite);
		victoryWindow.draw(congrats);
		victoryWindow.draw(options);

		victoryWindow.display();
	}
}


void Game::createGameMenu()
{
	using namespace sf;

	RenderWindow menuWindow(VideoMode(500, 400), "Menu", Style::Close);

	// creating a sprite for our menu background
	Texture menuBackgroundTexture;
	menuBackgroundTexture.loadFromFile("src/images/menu_background.jpg");
	Sprite menuBackgroundSprite;
	menuBackgroundSprite.setTexture(menuBackgroundTexture);

	Font font;
	font.loadFromFile("src/fonts/BakingLion.otf");

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
		Vector2i pos = Mouse::getPosition(menuWindow);

		Event event;

		while (menuWindow.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == Event::Closed)
				menuWindow.close();

			if (event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left) {
				if (pvpOption.getGlobalBounds().contains((float)pos.x, (float)pos.y)) {
					menuWindow.close();
					gameState = GAME_ON;
					play();
				}
				if (pvbotOption.getGlobalBounds().contains((float)pos.x, (float)pos.y)) {

				}
				if (pvpOnlineOption.getGlobalBounds().contains((float)pos.x, (float)pos.y)) {

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

}
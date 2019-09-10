#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>


enum PieceType 
{
	PAWN, 
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING
};

enum Color 
{
	WHITE,
	BLACK
};

#define Turn Color

struct Point
{
	int x;
	int y;
	Point(int a, int b) : x(a), y(b) {

	}
	Point() {
	
	}
	bool operator== (const Point& rhs) {
		return (x == rhs.x && y == rhs.y);
	}
	
	bool isOutOfBoard()
	{
		if (x > 7 || x < 0 || 
			y > 7 || y < 0)
			return true;

		return false;
	}

	bool isInitialized()
	{
		if (x == -1 && y == -1)
			return false;
		else
			return true;
	}
};


// here we load and store 12 textures and then create sprites
// for our chess pieces out of them
static sf::Sprite getNecessarySprite(Color color, PieceType type) {
	std::string absolutePath = "C:/Users/Vladimir/source/repos/Chess/Chess/src/";


	static sf::Texture whitePieces[6];
	static sf::Texture blackPieces[6];

	static bool texturesLoaded = false;
	if (!texturesLoaded) {
		// loading textures for white and black pieces only once
		for (int i = 0; i < 6; i++) {
			std::string fileName = absolutePath + "images/Pieces/White/" + std::to_string(i) + ".png";
			whitePieces[i].loadFromFile(fileName);
			whitePieces[i].setSmooth(true);
		}

		for (int i = 0; i < 6; i++) {
			std::string fileName = absolutePath + "images/Pieces/Black/" + std::to_string(i) + ".png";
			blackPieces[i].loadFromFile(fileName);
			blackPieces[i].setSmooth(true);
		}
		texturesLoaded = true;
	}

	sf::Sprite sprite;
	color == WHITE ? sprite.setTexture(whitePieces[type]) : sprite.setTexture(blackPieces[type]);

	return sprite;
}


class ChessPiece 
{
private:

	PieceType type;
	Color color;
	sf::Sprite sprite;

	/* each piece has value:
	PAWN - 1
	KNIGHT - 3
	BISHOP - 3.2 (a bit stronger than knight)
	ROOK - 5
	QUEEN - 8
	KING - 100 (actually priceless)
	*/
	float value; 

public:

	Color getColor() {
		return color;
	}

	PieceType getType() {
		return type;
	}

	sf::Sprite& getSprite() {
		return sprite;
	}

	float getValue() {
		return value;
	}

	ChessPiece(PieceType t, Color c, float v) :
		       type(t), color(c), value(v)
	{ 
		sprite = getNecessarySprite(c, t);
		sprite.scale(sf::Vector2f(1.4f, 1.4f));
	}
	~ChessPiece() {}

};
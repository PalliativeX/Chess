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
};

sf::Sprite getNecessarySprite(Color color, PieceType type);


class ChessPiece 
{
private:
	PieceType type;
	Color color;
	sf::Sprite sprite;
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

	ChessPiece(PieceType t, Color c) :
		       type(t), color(c) 
	{ 
		sprite = getNecessarySprite(c, t);
		sprite.scale(sf::Vector2f(1.4f, 1.4f));
	}
	~ChessPiece() {}
};
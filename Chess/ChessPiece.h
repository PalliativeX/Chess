enum PieceType {
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING
};

enum Color {
	WHITE,
	BLACK
};

#define Turn Color

struct Point {
	int x;
	int y;
	Point(int a, int b) : x(a), y(b) {}
};

class ChessPiece {
private:
	int value;
	PieceType type;
	Color color;
public:
	Color getColor() {
		return color;
	}

	int getValue() {
		return value;
	}
	PieceType getType() {
		return type;
	}
	ChessPiece(int v, PieceType t, Color c) :
		value(v), type(t), color(c) { }
	~ChessPiece() {}
};
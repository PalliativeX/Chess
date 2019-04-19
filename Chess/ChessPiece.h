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
	PieceType type;
	Color color;
public:
	Color getColor() {
		return color;
	}

	PieceType getType() {
		return type;
	}
	ChessPiece(PieceType t, Color c) :
		       type(t), color(c) { }
	~ChessPiece() {}
};
#pragma once
#include "Vec2.h"
#include "raylibCpp.h"
#include "Board.h"
#include <vector>
#include <assert.h>

class Tetromino {
public:
	enum class Rotation {
		UP,//compiler given them number values by defult
		RIGHT,
		DOWN,
		Left
	};
	Tetromino(const bool* shape, int dimension, Color c, const Board& board, const bool startingInMiddle, const int typeNumber);
	void Draw() const;
	void Move(Vec2<int> movement);
	void rotateClockwise();
	void rotateCounterClockwise();
	int getDimension() const;
	Vec2<int> getBoardPos() const;
	Color getColor() const;
	std::vector<int> getPositions() const;
	int GetTypeNumber() const;
private:
	Vec2<int> boardPos;
	//Vec2<int> currentPos;
	Rotation currentRotation;
	const bool* shape;
	const int typeNumber;
	const int dimension;
	const Color color;
	const Board& board;
};

class Straight : public Tetromino {
public:
	Straight(const Board& board, const bool startingInMiddle)
		:
		Tetromino(shape, dimension, color, board, startingInMiddle, typeNumber)
	{
		static_assert(sizeof(shape) / sizeof(bool) == dimension * dimension);
	}

private:
	static constexpr int typeNumber = 0;
	static constexpr bool shape[] = {0, 0, 0, 0,
									 1, 1, 1, 1,
									 0, 0, 0, 0,
									 0, 0, 0, 0};
	static constexpr int dimension = 4;
	static constexpr Color color = SKYBLUE;
};

class Square : public Tetromino {
public:
	Square(const Board& board, const bool startingInMiddle)
		:
		Tetromino(shape, dimension, color, board, startingInMiddle, typeNumber)
	{
		static_assert(sizeof(shape) / sizeof(bool) == dimension * dimension);
	}
private:
	static constexpr int typeNumber = 1;
	static constexpr bool shape[] = {1, 1,
									 1, 1};
	static constexpr int dimension = 2;
	static constexpr Color color = YELLOW;
};

class Tee : public Tetromino {
public:
	Tee(const Board& board, const bool startingInMiddle)
		:
		Tetromino(shape, dimension, color, board, startingInMiddle, typeNumber)
	{
		static_assert(sizeof(shape) / sizeof(bool) == dimension * dimension);
	}
private:
	static constexpr int typeNumber = 2;
	static constexpr bool shape[] = {0, 1, 0,
									 1, 1, 1,
									 0, 0, 0};
	static constexpr int dimension = 3;
	static constexpr Color color = MAGENTA;
};

class Jay : public Tetromino {
public:
	Jay(const Board& board, const bool startingInMiddle)
		:
		Tetromino(shape, dimension, color, board, startingInMiddle, typeNumber)
	{
		static_assert(sizeof(shape) / sizeof(bool) == dimension * dimension);
	}
private:
	static constexpr int typeNumber = 3;
	static constexpr bool shape[] = {1, 0, 0,
									 1, 1, 1,
									 0, 0, 0};
	static constexpr int dimension = 3;
	static constexpr Color color = DARKBLUE;
};

class El : public Tetromino {
public:
	El(const Board& board, const bool startingInMiddle)
		:
		Tetromino(shape, dimension, color, board, startingInMiddle, typeNumber)
	{
		static_assert(sizeof(shape) / sizeof(bool) == dimension * dimension);
	}
private:
	static constexpr int typeNumber = 4;
	static constexpr bool shape[] = {0, 0, 1,
									 1, 1, 1,
									 0, 0, 0};
	static constexpr int dimension = 3;
	static constexpr Color color = ORANGE;
};

class SkewZ : public Tetromino {
public:
	SkewZ(const Board& board, const bool startingInMiddle)
		:
		Tetromino(shape, dimension, color, board, startingInMiddle, typeNumber)
	{
		static_assert(sizeof(shape) / sizeof(bool) == dimension * dimension);
	}
private:
	static constexpr int typeNumber = 5;
	static constexpr bool shape[] = {1, 1, 0,
									 0, 1, 1,
									 0, 0, 0};
	static constexpr int dimension = 3;
	static constexpr Color color = GREEN;
};

class SkewS : public Tetromino {
public:
	SkewS(const Board& board, const bool startingInMiddle)
		:
		Tetromino(shape, dimension, color, board, startingInMiddle, typeNumber)
	{
		static_assert(sizeof(shape) / sizeof(bool) == dimension * dimension);
	}
private:
	static constexpr int typeNumber = 6;
	static constexpr bool shape[] = { 0, 1, 1,
									 1, 1, 0,
									 0, 0, 0 };
	static constexpr int dimension = 3;
	static constexpr Color color = RED;
};
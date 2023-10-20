#include "Tetrominoes.h"

Tetromino::Tetromino(const bool* shape, int dimension, Color c, const Board& board, const bool startingInMiddle, const int typeNumber)
	:
	shape(shape),
	dimension(dimension),
	color(c),
	board(board),
	currentRotation(Rotation::UP),
	typeNumber(typeNumber)
{
	if (startingInMiddle) {
		boardPos = { board.GetWidth() / 2 - dimension / 2, board.GetHeight() / 2 - dimension / 2 };
	}
	else {
		boardPos = { board.GetWidth() / 2 - dimension / 2, 0 };
	}
}

void Tetromino::Draw() const
{
	for (int y = 0; y < dimension; ++y) {
		for (int x = 0; x < dimension; ++x) {
			bool cell = false;
			switch (currentRotation)
			{
			case Tetromino::Rotation::UP:
				cell = shape[y * dimension + x];
				break;
			case Tetromino::Rotation::RIGHT:
				cell = shape[dimension * (dimension - 1)  - dimension * x + y];
				break;
			case Tetromino::Rotation::DOWN:
				cell = shape[(dimension * dimension - 1) - dimension * y - x];
				break;
			case Tetromino::Rotation::Left:
				cell = shape[dimension - 1 + dimension * x - y];
				break;
			default:
				break;
			}
			if (cell) {
				Vec2<int> drawPos = boardPos + Vec2<int>{x, y};
				board.DrawCell(drawPos, color);
			}
		}
	}
}

void Tetromino::Move(Vec2<int> movement)
{
	boardPos = boardPos + movement;
}

Vec2<int> Tetromino::getBoardPos() const
{
	return boardPos;
}

int Tetromino::getDimension() const
{
	return dimension;
}

Color Tetromino::getColor() const
{
	return color;
}

int Tetromino::GetTypeNumber() const
{
	return typeNumber;
}

void Tetromino::rotateClockwise()
{
	currentRotation = Rotation((int(currentRotation) + 1) % 4);
}

std::vector<int> Tetromino::getPositions() const
{
	std::vector<int> positions;
	for (int x = 0; x < dimension; x++) {
		for (int y = 0; y < dimension; y++) {
			bool cell = false;
			switch (currentRotation)
			{
			case Tetromino::Rotation::UP:
				cell = shape[y * dimension + x];
				break;
			case Tetromino::Rotation::RIGHT:
				cell = shape[dimension * (dimension - 1) - dimension * x + y];
				break;
			case Tetromino::Rotation::DOWN:
				cell = shape[(dimension * dimension - 1) - dimension * y - x];
				break;
			case Tetromino::Rotation::Left:
				cell = shape[dimension - 1 + dimension * x - y];
				break;
			default:
				break;
			}
			if (cell) {
				positions.push_back(x + y * board.GetWidth() + boardPos.GetX() + boardPos.GetY() * board.GetWidth());
			}
		}
	}
	return positions;
}


void Tetromino::rotateCounterClockwise()
{
	if (currentRotation == Rotation::UP) {
		currentRotation = Rotation::Left;
	}
	else {
		currentRotation = Rotation(int(currentRotation) - 1);
	}
}
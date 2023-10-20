#pragma once
#include <raylib.h>
#include <vector>
#include "Vec2.h"

class Board {

public:
	Board(Vec2<int> screenPos, Vec2<int> widthHeight, Vec2<int> backgroundBuffer, int cellsize_in, int padding_in);
	void SetCell(Vec2<int> pos, Color c);
	void DrawCell(Vec2<int> pos) const;
	void DrawCell(Vec2<int> pos, Color c) const;
	void Draw(Color c) const;
	void AddFallenTetromino(std::vector<int> positions, Color color);
	bool CellExists(Vec2<int> pos) const;
	int GetWidth() const;
	int GetHeight() const;
	int CheckLineClear();
	int GetLevel() const;
	
private:
	class Cell {

		public:
			Cell();
			void setColor(Color newColor);
			void setExists();
			void Remove();
			Color getColor() const;
			bool Exists() const;
		private:
			bool bExists;
			Color c;
	};

	int CalculateScore();
	int PosToIndex(Vec2<int> position) const;
	void DropRows(int dropFrom, int fallDistance);
	void DrawBackground(Color c) const;
	void OrderExistingCells();
	void RemoveCell(Vec2<int> position);

	std::vector<Cell> cells;
	std::vector<Vec2<int>> ExistingCells;//Stores all the x y positions of the Cells that have fallen and hit the bottom of the board
	std::vector<int> clears;
	int level;
	const int width;
	const int height;
	const int cellSize;
	const int padding;
	const Vec2<int> backgroundBuffer;
	Vec2<int> screenPos;
};
#include "Board.h"
#include "raylibCpp.h"
#include <assert.h>
#include <iostream>

Board::Cell::Cell()
{
	bExists = false;
	c = Color(RAYWHITE);
}

void Board::Cell::setColor(Color newColor)
{
	c = newColor;
	setExists();
}

void Board::Cell::setExists()
{
	bExists = true;
}

void Board::Cell::Remove()
{
	bExists = false;
}

Color Board::Cell::getColor() const
{
	return c;
}

bool Board::Cell::Exists() const
{
	return bExists;
}

Board::Board(Vec2<int> screenPos, Vec2<int> widthHeight, Vec2<int> backgroundBuffer, int cellSize_in, int padding_in)
	:
	screenPos(screenPos),
	cellSize(cellSize_in),
	padding(padding_in),
	width(widthHeight.GetX()),
	height(widthHeight.GetY()),
	backgroundBuffer(backgroundBuffer)

{
	assert(width > 0 && height > 0);
	// If assertion triggers : The width or height is smaller than 1
	assert(cellSize > 0);
	// If assertion triggers : The cell size is smaller than 1
	cells.resize(width * height);
	ExistingCells.clear();//Make sure the filled cells are clear (Could put a starting position here but would need function to actually update the cells in the cells array)
	clears.clear();
	level = 0;
	//if (height >= 17) {
	//	for (int j = 1; j <= 18; j++) {
	//		for (int i = 0; i < GetWidth() - 1; i++) {//DEBUGGING Set up almost complete row
	//			if (Vec2<int>{ i, GetHeight() - j } != Vec2<int>{80, 18}) {
	//				SetCell({ i, GetHeight() - j }, RAYWHITE);
	//				ExistingCells.push_back({ i, GetHeight() - j });
	//			}
	//		}
	//	}
	//}
}

void Board::SetCell(Vec2<int> pos, Color c)
{
	assert(pos.GetX() >= 0 && pos.GetY() >= 0 && pos.GetX() < width && pos.GetY() < height);
	//If assertion triggers : x or y is out of bounds
	int index = (width * pos.GetY()) + pos.GetX();
	cells[index].setExists();
	cells[index].setColor(c);
}

void Board::DrawCell(Vec2<int> pos) const
{
	assert(pos.GetX() >= 0 && pos.GetY() >= 0 && pos.GetX() < width && pos.GetY() < height);
	//If assertion triggers : x or y is out of bounds
	int index = (width * pos.GetY()) + pos.GetX();
	Color c = cells[index].getColor();
	DrawCell(pos, c);
}

void Board::DrawCell(Vec2<int> pos, Color c) const
{
	assert(pos.GetX() >= 0 && pos.GetY() >= 0 && pos.GetX() < width && pos.GetY() < height);
	//If assertion triggers : x or y is out of bounds
	Vec2<int> drawPos = screenPos + padding + (pos * cellSize);
	raycpp::DrawRectangle(drawPos, Vec2{ cellSize, cellSize } - padding, c);
}

void Board::Draw(Color c) const
{
	DrawBackground(c);
	for (int iY = 0; iY < height; ++iY) {
		for (int iX = 0; iX < width; ++iX) {
			if (CellExists(Vec2<int> {iX, iY})) {
				DrawCell(Vec2<int> {iX, iY});
			}
		}
	}
}

bool Board::CellExists(Vec2<int> pos) const
{
	int index = (width * pos.GetY()) + pos.GetX();
	return cells[index].Exists();
}

int Board::GetWidth() const
{
	return width;
}

int Board::GetHeight() const
{
	return height;
}

int Board::CheckLineClear()
{
	int rowY = -1, count = 0;
	bool giveUp = false;
	std::vector<int> indexToRemoveFrom;
	OrderExistingCells();
	//std::cout << "|| Existing Cells = ||";
	//for (int i = 0; i < ExistingCells.size(); i++) {
	//	std::cout << "{" << ExistingCells[i].GetX() << ", " << ExistingCells[i].GetY() << "} ";
	//}
	//std::cout << "|| Checking Line Clear ||";
	for (int i = 0; i < ExistingCells.size(); i++) {
		if (ExistingCells[i].GetY() != rowY) {
			//std::cout << " Failed row " << rowY << " | ";
			rowY = ExistingCells[i].GetY();
			count = 0;
			giveUp = false;
		}
		if (!giveUp) {
			//std::cout << " Counting " << ExistingCells[i].GetX() << " | ";
			if (ExistingCells[i].GetX() != count++) {
				//std::cout << " Failed Counting at " << count << " | ";
				giveUp = true;
			}
			if (count == GetWidth()) {
				//std::cout << " FOUND ROW!!! " << ExistingCells[(i - GetWidth() + 1)].GetX() << " , " << ExistingCells[(i - GetWidth() + 1)].GetY();
				indexToRemoveFrom.push_back(i - GetWidth() + 1);
			}
		}
	}
	std::vector<int> dropHeights;
	for (int i = indexToRemoveFrom.size() - 1; i >= 0; i--) {
		dropHeights.push_back(ExistingCells[indexToRemoveFrom[i]].GetY());
		for (int j = indexToRemoveFrom[i]; j < indexToRemoveFrom[i] + GetWidth(); j++) {
			//std::cout << " Removing index " << j << " with {" << ExistingCells[j].GetX() <<", "<< ExistingCells[j].GetY() << "} ";
			RemoveCell(ExistingCells[j]);//Set the cell to be empty
		}
		//std::cout << " Removing Row from exisiting array ";
		ExistingCells.erase(ExistingCells.begin() + indexToRemoveFrom[i], ExistingCells.begin() + indexToRemoveFrom[i] + GetWidth());//Remove the cell from the array of fallen cells (just holds their indexes)
	}
	for (int i = dropHeights.size() - 1; i >= 0; i--) {;
		//std::cout << "\n dropheights is " << dropHeights[i];
		DropRows(dropHeights[i], 1);
	}
	if (indexToRemoveFrom.size() != 0) {
		clears.push_back(indexToRemoveFrom.size());//Store how many lines were removed in one go
	}
	

	return CalculateScore();//Temp will return the updated score
}

int Board::GetLevel() const
{
	return level;
}

void Board::AddFallenTetromino(std::vector<int> positions, Color color)
{
	for (int i = 0; i < positions.size(); i++) {
		cells[positions[i]].setExists();
		cells[positions[i]].setColor(color);
		ExistingCells.push_back({positions[i] % GetWidth(), positions[i] / GetWidth()});//int to Vec2 helper function?
	}
}

int Board::CalculateScore()
{
	int score = 0, totalClears = 0;
	for (int i = 0; i < clears.size(); i++) {
		totalClears += clears[i];
		switch (clears[i]) {
		case 1:
			score += 40 * ((totalClears / 10) + 1);
			break;
		case 2:
			score += 100 * ((totalClears / 10) + 1);
			break;
		case 3:
			score += 300 * ((totalClears / 10) + 1);
			break;
		case 4:
			score += 1200 * ((totalClears / 10) + 1);
			break;
		}
	}
	level = totalClears / 10;
	return score;
}

int Board::PosToIndex(Vec2<int> position) const
{
	return position.GetY() * GetWidth() + position.GetX();
}

void Board::DropRows(int dropHeight, int fallDistance)
{
	Color c;
	for (int i = ExistingCells.size() - 1; i >= 0; i--) {
		if (ExistingCells[i].GetY() < dropHeight) {//Will not drop cells that were "above" the removed lines height
			c = cells[PosToIndex(ExistingCells[i])].getColor();
			cells[PosToIndex(ExistingCells[i])].Remove();
			cells[PosToIndex(ExistingCells[i] + Vec2<int>{0, fallDistance})].setColor(c);
			ExistingCells[i] = ExistingCells[i] + Vec2<int>{0, fallDistance};
		}
	}
}

void Board::DrawBackground(Color c) const
{
	Vec2<int> drawPos = screenPos - backgroundBuffer;
	Vec2<int> backgroundWidth = Vec2<int>{ width, height } *cellSize + Vec2<int>{padding, padding};
	raycpp::DrawRectangle(drawPos, backgroundWidth + backgroundBuffer * 2, c);
	raycpp::DrawRectangle(screenPos, backgroundWidth, BLACK);
}

void Board::OrderExistingCells()
{
	Vec2<int> temp;
	for (int i = 0; i < ExistingCells.size(); i++) {
		bool swappedSomething = false;
		for (int j = 0; j < ExistingCells.size() - i - 1; j++) {
			if (ExistingCells[j] > ExistingCells[j + 1]) {
				temp = ExistingCells[j];
				ExistingCells[j] = ExistingCells[j + 1];
				ExistingCells[j + 1] = temp;
				swappedSomething = true;
			}
		}
		if (!swappedSomething) {
			break;
		}
	}
}

void Board::RemoveCell(Vec2<int> position)
{
	cells[position.GetY() * GetWidth() + position.GetX()].Remove();
}

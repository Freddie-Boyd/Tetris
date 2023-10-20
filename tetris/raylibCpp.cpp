#include "raylibCpp.h"
#include <assert.h>

void raycpp::DrawRectangle(Vec2<int> pos, Vec2<int> widthHeight, Color color)
{
	assert(pos.GetX() >= 0 && pos.GetY() >= 0 &&
		pos.GetX() < GetScreenWidth() && pos.GetY() < GetScreenHeight());
	//if assertion triggers trying to draw outside of the screen
	DrawRectangle(pos.GetX(), pos.GetY(), widthHeight.GetX(), widthHeight.GetY(), color);
}

void raycpp::DrawText(const char* text, Vec2<int> pos, int textSize, Color color)
{
	DrawText(text, pos.GetX(), pos.GetY(), textSize, color);
}

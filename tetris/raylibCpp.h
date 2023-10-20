#pragma once
#include <raylib.h>
#include "Vec2.h"

namespace raycpp {

	void DrawRectangle(Vec2<int> pos, Vec2<int> widthHeight, Color color);                        // Draw a color-filled rectangle
	void DrawText(const char* text, Vec2<int> pos, int textSize, Color color);
}
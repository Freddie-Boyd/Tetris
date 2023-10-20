#pragma once
#include "Vec2.h"

namespace settings {
    //Window settings
    inline constexpr int screenWidth = 800;
    inline constexpr int screenHeight = 600;
    inline constexpr int maxFps = 60;
    inline constexpr const char* iconFilename = "TetrisIcon.png";

    //Board settings
    inline constexpr int cellSize = 23;
    inline constexpr int padding = 2;

    inline constexpr int scoreTextSize = 30;
    inline constexpr int finalScoreTextSize = 40;
    inline constexpr int levelTextSize = 20;
    inline constexpr int nextTetTextSize = 30;
    inline constexpr int storedTetTextSize = 25;
    inline constexpr int gameOverTextSize = 100;

    inline constexpr int moveRatePerTick = 10;
    inline constexpr int bufferPeriod = 2;

    inline constexpr Vec2<int> boardPosition{225, 70};
    inline constexpr Vec2<int> boardWidthHeight{ 10, 20 };
    inline constexpr Vec2<int> backgroundBuffer{10, 10};

    inline constexpr Vec2<int> scorePosition{ 510, 80 };
    inline constexpr Vec2<int> levelPosition{ 510, 115 };

    inline constexpr Vec2<int> gameOverPosition{94, 100};

    inline constexpr Vec2<int> finalScorePosition{ 155, 200 };

    inline constexpr Vec2<int> nextTetrominoText{ 510 , 160 };
    inline constexpr Vec2<int> nextTetrominoPosition{ 620, 160 };
    inline constexpr Vec2<int> nextTetrominoWidthHeight{ 6, 6 };
    inline constexpr Vec2<int> nextTetrominoBackgroundBuffer{ 6, 6 };

    inline constexpr Vec2<int> storedTetrominoText{ 510 , 360 };
    inline constexpr Vec2<int> storedTetrominoPosition{ 620, 360 };
    inline constexpr Vec2<int> storedTetrominoWidthHeight{ 6, 6 };
    inline constexpr Vec2<int> storedTetrominoBackgroundBuffer{ 6, 6 };
}
#include "raylib.h"
#include "Game.h"
#include "Settings.h"

int main(void)
{
    

    Game game{settings::screenWidth, settings::screenHeight, settings::maxFps, "Tetris"};
    while (!game.ShouldGameClose()) {
        game.Tick();
    }
    return 0;
}
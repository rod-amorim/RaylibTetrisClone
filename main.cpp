#include <raylib.h>
#include <iostream>
#include "game.h"
#include "colors.h"

double lastUpdatedTime = 0;

bool EventTriggered(double interval)
{
    double currentGameTime = GetTime();
    if (currentGameTime - lastUpdatedTime >= interval)
    {
        lastUpdatedTime = currentGameTime;
        return true;
    }
    return false;
}

int main()
{
    InitWindow(500, 620, "Raylib Tetris");

    SetTargetFPS(60);

    Font font = LoadFontEx("src/font/monogram.ttf", 64, 0, 0);

    Game game = Game();

    while (WindowShouldClose() == false)
    {
        UpdateMusicStream(game.music);
        if (EventTriggered(0.5))
        {
            game.MoveBlockDown();
        }

        game.HandleInput();
        BeginDrawing();

        ClearBackground(darkBlue);

        if (game.gameOver)
        {
            DrawTextEx(font, "GAME OVER", {320, 450}, 38, 2, WHITE);
        }

        // score text
        DrawTextEx(font, "Score", {365, 15}, 38, 2, WHITE);
        DrawRectangleRounded({320, 55, 170, 60}, 0.3, 6, lightBlue);

        // current score value
        char scoreText[10];
        sprintf(scoreText, "%d", game.score);
        Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
        DrawTextEx(font, scoreText, {320 + (170 - textSize.x) / 2, 65}, 38, 2, WHITE);
        DrawRectangleRounded({320, 215, 170, 180}, 0.3, 6, lightBlue);

        // next text
        DrawTextEx(font, "Next", {370, 175}, 38, 2, WHITE);
        // draw next block

        game.Draw();

        EndDrawing();
    }

    CloseWindow();
}
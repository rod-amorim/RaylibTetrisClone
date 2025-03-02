#pragma once
#include "grid.h"
#include "blocks.cpp"

class Game
{
public:
    bool gameOver;
    Game();
    ~Game();
    void MoveBlockDown();
    void HandleInput();
    void Draw();
    int score;
    Music music;

private:
    Grid grid;
    void MoveBlockLeft();
    void MoveBlockRight();
    Block GetRandomBlock();
    std::vector<Block> GetAllBlcoks();
    bool IsBlockOutside();
    void Rotate();
    void LockBlock();
    bool BlockFits();
    void Reset();
    void UpdateScore(int linesCleared, int moveDownPoints);
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    Sound rotateSound;
    Sound clearSound;
};

#include "game.h"
#include <random>

Game::Game()
{
    score = 0;
    grid = Grid();
    blocks = GetAllBlcoks();
    gameOver = false;
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();

    InitAudioDevice();

    music = LoadMusicStream("src/sounds/music.wav");
    rotateSound = LoadSound("src/sounds/rotate.wav");
    clearSound = LoadSound("src/sounds/clear.wav");

    PlayMusicStream(music);
}

Game::~Game()
{
    UnloadMusicStream(music);
    UnloadSound(rotateSound);
    UnloadSound(clearSound);

    CloseAudioDevice();
}

void Game::HandleInput()
{
    int key = GetKeyPressed();

    if (gameOver && key != 0)
    {
        gameOver = false;
        Reset();
        return;
    }

    switch (key)
    {
    case KEY_LEFT:
        MoveBlockLeft();
        break;
    case KEY_RIGHT:
        MoveBlockRight();
        break;
    case KEY_DOWN:
        MoveBlockDown();
        UpdateScore(0, 1);
        break;
    case KEY_UP:
        Rotate();

        break;

    default:
        break;
    }
}

void Game::MoveBlockLeft()
{
    if (gameOver == true)
    {
        return;
    }
    currentBlock.Move(0, -1);
    if (IsBlockOutside() || BlockFits() == false)
    {
        currentBlock.Move(0, 1);
    }
}

void Game::MoveBlockRight()
{
    if (gameOver == true)
    {
        return;
    }
    currentBlock.Move(0, 1);
    if (IsBlockOutside() || BlockFits() == false)
    {
        currentBlock.Move(0, -1);
    }
}

void Game::MoveBlockDown()
{
    if (gameOver == true)
    {
        return;
    }
    currentBlock.Move(1, 0);
    if (IsBlockOutside() || BlockFits() == false)
    {
        currentBlock.Move(-1, 0);
        LockBlock();
    }
}

void Game::Rotate()
{
    if (gameOver == true)
    {
        return;
    }
    currentBlock.Rotate();
    if (IsBlockOutside() || BlockFits() == false)
    {
        currentBlock.UndoRotate();
    }
    else
    {
        PlaySound(rotateSound);
    }
}

Block Game::GetRandomBlock()
{
    if (blocks.empty())
    {
        blocks = GetAllBlcoks();
    }

    int randIndex = rand() % blocks.size();
    Block block = blocks[randIndex];
    blocks.erase(blocks.begin() + randIndex);
    return block;
}

std::vector<Block> Game::GetAllBlcoks()
{
    return {IBlock(),
            JBlock(),
            LBlock(),
            OBlock(),
            SBlock(),
            TBlock(),
            ZBlock()};
}

void Game::Draw()
{
    grid.Draw();
    currentBlock.Draw(11, 11);
    switch (nextBlock.id)
    {
    case 3:
        nextBlock.Draw(255, 290);
        break;
    case 4:
        nextBlock.Draw(255, 280);
        break;
    default:
        nextBlock.Draw(270, 270);
        break;
    }
}

bool Game::IsBlockOutside()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position pos : tiles)
    {
        if (grid.isCellOutside(pos.row, pos.col))
        {
            return true;
        }
    }
    return false;
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();

    for (Position pos : tiles)
    {
        grid.grid[pos.row][pos.col] = currentBlock.id;
    }
    currentBlock = nextBlock;

    if (BlockFits() == false)
    {
        gameOver = true;
    }

    nextBlock = GetRandomBlock();
    int rowsClreared = grid.ClearFullRows();

    if (rowsClreared > 0)
    {
        PlaySound(clearSound);
        UpdateScore(rowsClreared, 0);
    }
}

bool Game::BlockFits()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();

    for (Position pos : tiles)
    {
        if (grid.isCellEmpty(pos.row, pos.col) == false)
        {
            return false;
        }
    }
    return true;
}

void Game::Reset()
{
    grid.Initialize();
    blocks = GetAllBlcoks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    score = 0;
}

void Game::UpdateScore(int linesCleared, int moveDownPoints)
{
    switch (linesCleared)
    {
    case 1:
        score += 100;
        break;
    case 2:
        score += 300;
        break;
    case 3:
        score += 500;
        break;
    default:
        break;
    }

    score += moveDownPoints;
}

#include <random>
#include <fstream>
#include <algorithm>
#include <stack>

#include "board.h"

/* ------------------------------ Constructors ------------------------------ */

Board::Board(unsigned int mines) : totalMines{mines}
{
    if (totalMines > WIDTH * HEIGHT)
        throw std::runtime_error("ERROR: Number of mines exceeds total possible tile locations.");
    unrevealedTileCount = (WIDTH * HEIGHT) - totalMines;

    init();

    // Place mines in random positions
    std::vector<std::pair<int, int>> availablePositions;
    for (int x = 0; x < HEIGHT; ++x)
        for (int y = 0; y < WIDTH; ++y)
            availablePositions.push_back(std::make_pair(x, y));

    std::shuffle(availablePositions.begin(), availablePositions.end(), Random::getGenerator());

    for (int i = 0; i < mines; ++i)
    {
        int randX = availablePositions[i].first;
        int randY = availablePositions[i].second;
        tiles[randX][randY]->setMine();
    }

    initAdjacentMines();
}

Board::Board(const std::string &file) : totalMines{0}
{
    init();

    std::ifstream boardFile(file);
    if (!boardFile.is_open())
        throw std::runtime_error("The file " + file + " could not be opened.");

    std::string line;
    int i = 0;
    while (getline(boardFile, line))
    {
        if (line.length() != WIDTH)
            throw std::runtime_error("ERROR: Number of columns exceeds WIDTH of " + std::to_string(WIDTH));

        // '1's indicate a mine
        for (int j = 0; j < line.length(); ++j)
        {
            if (line[j] == '1')
            {
                tiles[i][j]->setMine();
                ++totalMines;
            }
        }

        if (i >= HEIGHT)
            throw std::runtime_error("ERROR: Number of rows exceeds HEIGHT of " + std::to_string(HEIGHT));

        ++i;
    }

    unrevealedTileCount = (WIDTH * HEIGHT) - totalMines;

    initAdjacentMines();
}

void Board::init()
{
    debugON = false;
    flagCount = 0;

    // Initialize the sprite textures and positions (starting game state)
    faceBtn.setTexture(*Textures::getTexture(FACE_PLAY_PNG));
    faceType = FACE_PLAY;
    faceBtn.setPosition(((WIDTH * IMAGESIZE) / 2) - IMAGESIZE, HEIGHT * IMAGESIZE);

    debugBtn.setTexture(*Textures::getTexture(DEBUG_BTN_PNG));
    debugBtn.setPosition((WIDTH * IMAGESIZE) - (10 * IMAGESIZE), HEIGHT * IMAGESIZE);

    debugMine.setTexture(*Textures::getTexture(TILE_MINE_PNG));

    digit.setTexture(*Textures::getTexture(DIGITS_PNG));

    for (int i = 0; i < NUM_TESTS; ++i)
    {
        int offset = 8 - i * 2; // Offset from right of the board
        testBtns[i].setTexture(*Textures::getTexture(TEST_PNG_PREFIX + std::to_string(i + 1) + ".png"));
        testBtns[i].setPosition(WIDTH * IMAGESIZE - (offset * IMAGESIZE), HEIGHT * IMAGESIZE);
    }

    // Initialize tiles in the grid
    for (int i = 0; i < HEIGHT; ++i)
    {
        std::vector<BoardTile *> tilesRow;
        for (int j = 0; j < WIDTH; ++j)
        {
            BoardTile *tile = new BoardTile(i, j);
            tilesRow.push_back(tile);
        }
        tiles.push_back(tilesRow);
    }
}

void Board::initAdjacentMines()
{
    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            int mines = 0;
            for (BoardTile *adjacent : getAdjacentTiles(i, j))
            {
                if (adjacent->isMine())
                    ++mines;
            }

            tiles[i][j]->setAdjacentMineCount(mines);
        }
    }
}

Board::BoardTile::BoardTile(unsigned int x, unsigned int y) : Tile{x, y} {}

/* -------------------------------- Accessors ------------------------------- */

const sf::Sprite &Board::getFaceButton() const { return faceBtn; }
const sf::Sprite *Board::getTestButtons() const { return testBtns; }
const sf::Sprite &Board::getDebugButton() const { return debugBtn; }
int Board::getFace() const { return faceType; }

// Private Helper Accessor

std::vector<Board::BoardTile *> Board::getAdjacentTiles(int x, int y)
{
    std::vector<BoardTile *> adjacentTiles;

    const int row[]{-1, -1, -1, 1, 1, 1, 0, 0};
    const int col[]{-1, 0, 1, -1, 0, 1, -1, 1};

    for (int i = 0; i < 8; ++i)
    {
        if (x + row[i] >= 0 && x + row[i] < HEIGHT && y + col[i] >= 0 && y + col[i] < WIDTH)
            adjacentTiles.push_back(tiles[x + row[i]][y + col[i]]);
    }

    return adjacentTiles;
}

/* -------------------------------- Mutators -------------------------------- */

void Board::toggleDebug() { debugON = !debugON; }

void Board::flagTile(int row, int col)
{
    BoardTile *tile = tiles.at(row).at(col);
    if (tile->isRevealed())
        return;

    tile->toggleFlag();

    if (tile->isFlagged())
        ++flagCount;
    else
        --flagCount;
}

void Board::revealTile(int row, int col)
{
    BoardTile *tile = tiles.at(row).at(col);
    if (tile->isFlagged() || tile->isRevealed())
        return;

    tile->revealTile();
    --unrevealedTileCount;

    if (tile->isMine())
    {
        setFace(FACE_LOSE);
        return;
    }

    // Use a stack to process adjacent tiles
    std::stack<BoardTile *> tilesToProcess;
    for (BoardTile *adjacent : getAdjacentTiles(tile->x, tile->y))
        tilesToProcess.push(adjacent);

    revealTiles(tilesToProcess);

    if (unrevealedTileCount == 0)
    {
        setFace(FACE_WIN);
    }
}

void Board::revealTiles(std::stack<BoardTile *> &tilesToProcess)
{
    while (!tilesToProcess.empty())
    {
        BoardTile *curr = tilesToProcess.top();
        tilesToProcess.pop();

        // Ignore mines, flags, and already revealed tiles
        if (curr->isMine() || curr->isFlagged() || curr->isRevealed())
            continue;

        curr->revealTile();
        --unrevealedTileCount;

        // Don't reveal more tiles if current had adjacent mines
        if (curr->getAdjacentMineCount() != 0)
            continue;

        for (BoardTile *adjacent : getAdjacentTiles(curr->x, curr->y))
        {
            tilesToProcess.push(adjacent);
        }
    }
}

// Private Helper Mutator

void Board::setFace(int type)
{
    faceType = type;
    switch (type)
    {
    case FACE_WIN:
        faceBtn.setTexture(*Textures::getTexture(FACE_WIN_PNG));
        break;
    case FACE_LOSE:
        faceBtn.setTexture(*Textures::getTexture(FACE_LOSE_PNG));
        break;
    case FACE_PLAY:
        faceBtn.setTexture(*Textures::getTexture(FACE_PLAY_PNG));
        break;
    default:
        throw std::runtime_error("ERROR: Unkown face type.");
    }
}

/* --------------------------------- Display -------------------------------- */

void Board::drawUpdates()
{
    // Draw buttons
    Window::window.draw(debugBtn);
    Window::window.draw(faceBtn);
    for (int i = 0; i < NUM_TESTS; ++i)
        Window::window.draw(testBtns[i]);

    // Draw tiles
    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            Window::window.draw(tiles[i][j]->getTileSprite());
            Window::window.draw(tiles[i][j]->getOverlaySprite());
            if (debugON && tiles[i][j]->isMine())
            {
                debugMine.setPosition(j * IMAGESIZE, i * IMAGESIZE);
                Window::window.draw(debugMine);
            }
        }
    }

    drawFlagCounter();

    if (!debugON)
        return;
}

void Board::drawFlagCounter()
{
    if (faceType == FACE_WIN)
    {
        drawDigit(0, 0);
        drawDigit(0, DIGITS_PNG_OFFSET);
        return;
    }

    int mineCount = totalMines - flagCount;
    int ones = abs(mineCount) % 10;
    int tens = (abs(mineCount) / 10) % 10;
    int hundreds = (abs(mineCount) / 100) % 10;
    int xPos = 0;

    // Draw negative sign if there are more flags than mines
    if (mineCount < 0)
    {
        drawDigit(DIGITS_NEGATIVE_SIGN, xPos);
        xPos += DIGITS_PNG_OFFSET;
    }

    if (hundreds != 0)
    {
        drawDigit(hundreds, xPos);
        xPos += DIGITS_PNG_OFFSET;
    }

    drawDigit(tens, xPos);
    xPos += DIGITS_PNG_OFFSET;
    drawDigit(ones, xPos);
}

void Board::drawDigit(int value, int xPos)
{
    digit.setTextureRect(sf::IntRect(DIGITS_PNG_OFFSET * value, 0, DIGITS_PNG_OFFSET, IMAGESIZE));
    digit.setPosition(xPos, HEIGHT * IMAGESIZE);
    Window::window.draw(digit);
}
#include "tile.h"

/* ------------------------------ Constructors ------------------------------ */

Tile::Tile(unsigned int xPos, unsigned int yPos)
    : x{xPos}, y{yPos},
      revealed{false}, flagged{false}, mine{false},
      adjacentMineCount{0}
{
    tileSprite.setPosition(y * IMAGESIZE, x * IMAGESIZE);
    tileSprite.setTexture(*Textures::getTexture(TILE_HIDDEN_PNG));

    overlaySprite.setPosition(y * IMAGESIZE, x * IMAGESIZE);
    overlaySprite.setTexture(*Textures::getTexture(TILE_HIDDEN_PNG));
}

/* -------------------------------- Accessors ------------------------------- */

bool Tile::isRevealed() const { return revealed; }
bool Tile::isFlagged() const { return flagged; }
bool Tile::isMine() const { return mine; }
unsigned int Tile::getAdjacentMineCount() const { return adjacentMineCount; }
const sf::Sprite &Tile::getTileSprite() const { return tileSprite; }
const sf::Sprite &Tile::getOverlaySprite() const { return overlaySprite; }

/* -------------------------------- Mutators -------------------------------- */

void Tile::setMine() { mine = true; }

void Tile::setAdjacentMineCount(unsigned int mines) { adjacentMineCount = mines; }

void Tile::revealTile()
{
    revealed = true;
    tileSprite.setTexture(*Textures::getTexture(TILE_REVEALED_PNG));

    if (mine)
        overlaySprite.setTexture(*Textures::getTexture(TILE_MINE_PNG));
    else if (adjacentMineCount == 0)
        overlaySprite.setTexture(*Textures::getTexture(TILE_REVEALED_PNG));
    else
        overlaySprite.setTexture(*Textures::getTexture(TILE_NUMBER_PNG_PREFIX + std::to_string(adjacentMineCount) + ".png"));
}

void Tile::toggleFlag()
{
    flagged = !flagged;

    if (flagged)
        overlaySprite.setTexture(*Textures::getTexture(TILE_FLAG_PNG));
    else
        overlaySprite.setTexture(*Textures::getTexture(TILE_HIDDEN_PNG));
}
#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>

#include <vector>

#include "textures.h"

/// @brief Represents a single tile in the Minesweeper board.
class Tile
{
public:
    const unsigned int x; // The x position of the tile on the board
    const unsigned int y; // The y position of the tile on the board

    /* ------------------------------ Constructors ------------------------------ */

    /// @brief Construct a Tile object at row `x` and column `y` on the board. Default (0, 0)
    /// @param x Row x
    /// @param y Column y
    Tile(unsigned int x = 0, unsigned int y = 0);

    /* -------------------------------- Accessors ------------------------------- */

    /// @return `true` if the tile is revealed; `false` otherwise.
    bool isRevealed() const;

    /// @return `true` if the tile is flagged; `false` otherwise.
    bool isFlagged() const;

    /// @return `true` if the tile is a mine; `false` otherwise.
    bool isMine() const;

    /// @return The number of adjacent mines to the tile.
    unsigned int getAdjacentMineCount() const;

    /* -------------------------------- Mutators -------------------------------- */

    /// @brief Mark the tile as a mine.
    void setMine();

    /// @brief Set the number of adjacent mines for the tile.
    /// @param mines The number of adjacent mines.
    void setAdjacentMineCount(unsigned int mines);

    /// @brief Mark the tile as revealed and update the sprites and window
    /// @param window The SFML render window.
    void revealTile(sf::RenderWindow &window);

    /// @brief Mark the tile as flagged or not and update the sprites and window
    /// @param flag Flagged if `true`, not flagged if `false`.
    /// @param window The SFML render window.
    void flagTile(bool flag, sf::RenderWindow &window);

    /* --------------------------------- Display -------------------------------- */

    /// @brief Update the tile in the window.
    /// @param window The SFML render window.
    void update(sf::RenderWindow &window);

private:
    bool revealed; // Tile is revealed
    bool flagged;  // Tile is flagged
    bool mine;     // Tile is a mine

    sf::Sprite tileSprite;    // The tile sprite
    sf::Sprite overlaySprite; // The overlayed tile sprite (flagging and debug mine)

    unsigned int adjacentMineCount; // Number of adjacent tiles that are mines
};

#endif // TILE_H
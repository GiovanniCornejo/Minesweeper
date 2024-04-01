#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>

#include <stack>

#include "textures.h"
#include "tile.h"
#include "window.h"
#include "random.h"

#define FACE_WIN 1   // Board is in win state
#define FACE_PLAY 0  // Board is in playable state
#define FACE_LOSE -1 // Board is in lose state

/// @brief Manages the game board and tile interactions in Minesweeper.
class Board
{
public:
    static constexpr unsigned int WIDTH = 25;    // The width of the board
    static constexpr unsigned int HEIGHT = 16;   // The height of the board
    static constexpr unsigned int NUM_TESTS = 3; // Number of test buttons for the board

    /* ------------------------------ Constructors ------------------------------ */

    /// @brief Construct a Board object with randomly placed `mines. Default 50
    /// @param mines The number of mines on the board.
    Board(unsigned int mines = 50);

    /// @brief Construct a Board object from a text file comprised of a
    ///        16 x 25 grid of zeros (normal tiles) and ones (mines).
    /// @param file The path to the text file.
    Board(const std::string &file);

    /* -------------------------------- Accessors ------------------------------- */

    /// @return The face button sprite.
    const sf::Sprite &getFaceButton() const;

    /// @return A pointer to the test button sprites array.
    /// Use `NUM_TESTS` for traversal.
    const sf::Sprite *getTestButtons() const;

    /// @return The debug button sprite.
    const sf::Sprite &getDebugButton() const;

    /// @return The current face type (Lose/Win/Playable).
    int getFace() const;

    /* -------------------------------- Mutators -------------------------------- */

    /// @brief Toggle the debug mode. Display all mines on the board.
    void toggleDebug();

    /// Flag the tile at the specified indices. Does nothing if the tile is already revealed.
    /// @param row The row index of the tile.
    /// @param col The column index of the tile.
    void flagTile(int row, int col);

    /// @brief Reveal the tile at the specified indices.
    ///        Also reveals adjacent tiles that are not flagged or mines.
    ///        Does nothing if the tile is flagged or already revealed.
    /// @param row The row index of the tile.
    /// @param col The column index of the tile.
    void revealTile(int row, int col);

    /* --------------------------------- Display -------------------------------- */

    /// @brief Update the display of all sprites that changed during gameplay.
    void drawUpdates();

private:
    /// @brief Represents a tile on the game board, providing access to specific functionality for the Board class.
    struct BoardTile : public Tile
    {
        /// @brief Construct a BoardTile object at row `x` and column `y` on the board.
        /// @param x The row-index of the tile.
        /// @param y The col-index of the tile.
        BoardTile(unsigned int x, unsigned int y);

        friend class Board;
    };

    std::vector<std::vector<BoardTile *>> tiles; // 2D vector of all tiles in the grid

    unsigned int unrevealedTileCount; // The number of unrevealed tiles (excluding mines)
    unsigned int totalMines;          // The total number of mines on the board
    int flagCount;                    // The number of flagged tiles
    int faceType;                     // Type of face displayed: `FACE_PLAY, `FACE_LOSE`, `FACE_WIN`
    bool debugON;                     // Turn debug mode on and off

    sf::Sprite digit;               // Counter for number of bombs left
    sf::Sprite faceBtn;             // Face that can change emotion
    sf::Sprite testBtns[NUM_TESTS]; // Buttons for tests and debug
    sf::Sprite debugBtn;            // Debug shows all mines
    sf::Sprite debugMine;           // Mine used to display all mines in debug mode

    /// @brief Constructor helper, initialize basic values and 2D vector of tiles.
    void init();

    /// @brief Constructor helper, initialize the number of adjacent mines for each tile.
    void initAdjacentMines();

    /// @brief Reveal helper, reveal all adjacent tiles that are not mines.
    void revealTiles(std::stack<BoardTile *> &tilesToProcess);

    /// @return Vector of pointers to the adjacent tiles at position `x`, `y`.
    std::vector<BoardTile *> getAdjacentTiles(int x, int y);

    /// @brief Update the current face type.
    void setFace(int type);

    /// @brief Draw helper, draw the flag counter to the SFML window.
    void drawFlagCounter();

    /// @brief Draw helper, draw a digit in the flag counter.
    void drawDigit(int value, int xPos);
};
#endif // BOARD_H
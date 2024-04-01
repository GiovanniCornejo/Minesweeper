#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <SFML/Graphics.hpp>

#include "board.h"
#include "textures.h"
#include "window.h"

#define TEST_BRD_PATH "../data/boards/"           // Relative path to test board file folder
#define TEST_BRD_PREFIX TEST_BRD_PATH "testboard" // Add character number 1-3.brd to this

/* ------------------------------- Application ------------------------------ */

/// @brief Run the game until the window is closed.
void runGame();

/// @brief Process user events that occur in the window during gameplay.
/// @param board The game board that updates its state based on user events.
void processEvents(Board &board);

/* ------------------------------ Mouse Action ------------------------------ */

/// @brief Check for right-clicks to flag tiles on the game board.
/// @param mousePos The position of the mouse cursor within the game window.
/// @param board The game board.
void rightClick(const sf::Vector2f &mousePos, Board &board);

/// @brief Check for left-clicks to reveal tiles or perform actions on the game board.
/// @param mousePos The position of the mouse cursor within the game window.
/// @param board The game board that updates its state based on the entity clicked.
void leftClick(const sf::Vector2f &mousePos, Board &board);

/* ----------------------------- Mouse Position ----------------------------- */

/// @brief Check if the mouse cursor is within the game area.
/// @param mousePos The position of the mouse cursor relative to the game window.
/// @param board The game board.
/// @return `true` if the mouse is in the bounds of the game board; `false` otherwise.
bool mouseInGame(const sf::Vector2f &mousePos, Board &board);

/// @brief Check if the mouse cursor is over a specific sprite
/// @param mousePos The position of the mouse cursor relative to the game window.
/// @param sprite The sprite to check against the mouse cursor position.
/// @return `true` if the mouse is over the sprite; `false` otherwise.
bool mouseOverSprite(const sf::Vector2f &mousePos, const sf::Sprite &sprite);

#endif // MINESWEEPER_H
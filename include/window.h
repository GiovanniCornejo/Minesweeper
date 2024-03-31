
#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>

struct Window
{
    static sf::RenderWindow window; // The main RenderWindow object

    /// @brief Initializes the application window.
    static void initializeWindow();
};

#endif // WINDOW_H
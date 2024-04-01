#include "window.h"

sf::RenderWindow Window::window;

void Window::initializeWindow()
{
    window.create(sf::VideoMode(800u, 600u), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);
}
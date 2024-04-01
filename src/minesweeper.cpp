#include "minesweeper.h"

// TODO: Run valgrind on this and check for leaks

int main()
{
    Window::initializeWindow();

    runGame();

    return 0;
}

/* ------------------------------- Application ------------------------------ */

void runGame()
{
    Board board = Board{};

    while (Window::window.isOpen())
    {
        Window::window.clear(sf::Color::White);

        processEvents(board);

        board.drawUpdates();

        Window::window.display();
    }
}

void processEvents(Board &board)
{
    for (sf::Event event = sf::Event{}; Window::window.pollEvent(event);)
    {
        switch (event.type)
        {
            /* Window-Based Events */

        case sf::Event::Closed:
        {
            Window::window.close();
            break;
        }

            /* Gameplay-Based Events */

        case sf::Event::MouseButtonReleased:
        {
            sf::Vector2f mousePos = Window::window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

            // Right Click (Flagging)
            if (board.getFace() == FACE_PLAY && event.mouseButton.button == sf::Mouse::Right)
                rightClick(mousePos, board);
            // Left Click (Buttons / Revealing)
            else if (event.mouseButton.button == sf::Mouse::Left)
                leftClick(mousePos, board);
            break;
        }
        }
    }
}

/* ------------------------------ Mouse Action ------------------------------ */

void rightClick(const sf::Vector2f &mousePos, Board &board)
{
    if (!mouseInGame(mousePos, board))
        return;

    int col = mousePos.x / IMAGESIZE;
    int row = mousePos.y / IMAGESIZE;

    board.flagTile(row, col);
}

void leftClick(const sf::Vector2f &mousePos, Board &board)
{
    // Tile clicked (Revealing)
    if (board.getFace() == FACE_PLAY && mouseInGame(mousePos, board))
    {
        int col = mousePos.x / IMAGESIZE;
        int row = mousePos.y / IMAGESIZE;

        board.revealTile(row, col);
    }

    // Face clicked (Restart)
    else if (mouseOverSprite(mousePos, board.getFaceButton()))
    {
        board = Board{};
    }

    // Debug clicked
    else if (board.getFace() == FACE_PLAY && mouseOverSprite(mousePos, board.getDebugButton()))
    {
        board.toggleDebug();
    }

    // Test clicked
    else
    {
        const sf::Sprite *testButtons = board.getTestButtons();
        for (int i = 0; i < board.NUM_TESTS; ++i)
        {
            if (mouseOverSprite(mousePos, testButtons[i]))
            {
                board = Board{TEST_BRD_PREFIX + std::to_string(i + 1) + ".brd"};
                break;
            }
        }
    }
}

/* ----------------------------- Mouse Position ----------------------------- */

bool mouseInGame(const sf::Vector2f &mousePos, Board &board)
{
    return (mousePos.x >= 0 && mousePos.x < board.WIDTH * IMAGESIZE &&
            mousePos.y >= 0 && mousePos.y < board.HEIGHT * IMAGESIZE);
}

bool mouseOverSprite(const sf::Vector2f &mousePos, const sf::Sprite &sprite)
{
    sf::Vector2f spritePos = sprite.getPosition();
    sf::FloatRect globalBounds = sprite.getGlobalBounds();
    sf::IntRect spriteBounds((int)spritePos.x, (int)spritePos.y,
                             (int)globalBounds.width, (int)globalBounds.height);

    return spriteBounds.contains(mousePos.x, mousePos.y);
}
#include <TicTacToe.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) && !defined(__CYGWIN__)

// Entry point for a Windows application (unicode)
int wWinMain()
{
#if defined(DEBUG) | defined(_DEBUG)
    // Enable run-time memory check for debug builds.
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}

#else
#error Only Windows is supported
#endif

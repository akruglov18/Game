#include "map_menu.h"

map_menu::map_menu() {
    color = sf::Color(255, 0, 0);
}

View_mode map_menu::Run(sf::RenderWindow& window, std::shared_ptr<Player> player) {
    while (true) {
        sf::Event event;
        window.pollEvent(event);

        if (event.type == sf::Event::Closed)
            return View_mode::EXIT;
        if (event.type == sf::Event::MouseButtonPressed)
            return View_mode::GAME;

        window.clear(color);
        window.display();
    }
}

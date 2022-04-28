#pragma once
#include "menu_button.h"
#include "screen.h"

class GameoverMenu : public Screen {
public:
    GameoverMenu();
    sf::Color color;
    sf::Text gameover_text;
    sf::Texture gameover_screenIMG;
    sf::Sprite gameover_screen;

    MenuButton b_restart;
    MenuButton b_main_menu;

    std::vector<MenuButton*> buttons;

    virtual View_mode Run(sf::RenderWindow& window, std::shared_ptr<Player> player = nullptr) override;
};

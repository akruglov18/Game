#include "game_ui.h"

GameUI::GameUI() {
    font = *Resources::FontsHolder::getResource("basic_font");
    UI_health.setFont(font);
    UI_health.setFillColor(sf::Color(255, 0, 0));
    UI_health.setCharacterSize(40);
    UI_health.setStyle(sf::Text::Bold);
    UI_health.setPosition(sf::Vector2f(20, 660));
}

void GameUI::update_UI(Player& p) {
    UI_health.setString("Health: " + std::to_string(p.get_health()));
}

void GameUI::show_UI(sf::RenderWindow& window) {
    window.draw(UI_health);
}

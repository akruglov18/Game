#include "trade_menu.h"

TradeMenu::TradeMenu(): b_exit("Back", sf::FloatRect({20.f, 20.f}, {150.f, 52.f}), View_mode::GAME) {
    color = sf::Color(240, 164, 99);
    buttons.push_back(&b_exit);

    gr_money_trader.setFont(font);
    gr_money_trader.setCharacterSize(40);
    gr_money_trader.setStyle(sf::Text::Bold);
    gr_money_trader.setFillColor(sf::Color(39, 177, 19));
    gr_money_trader.setPosition({700.f, 130.f});
}

View_mode TradeMenu::Run(sf::RenderWindow& window) {
    inventory_screenIMG.loadFromFile("../../images/tmp.jpg");
    inventory_screen.setTexture(inventory_screenIMG);
    inventory_screen.setColor(sf::Color(210, 164, 120, 130));

    sf::Event event;
    window.pollEvent(event);
    event.type = sf::Event::GainedFocus;
    View_mode to_return{View_mode::NONE};
    window.setView(window.getDefaultView());
    while (true) {
        window.pollEvent(event);

        to_return = MenuButton::buttons_checker(sf::Mouse::getPosition(window), buttons, event);

        if (event.type == sf::Event::Closed) {
            std::remove("../../images/tmp.jpg");
            to_return = View_mode::EXIT;
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::T) {
                std::remove("../../images/tmp.jpg");
                to_return = View_mode::GAME;
            }
        }

        if (to_return != View_mode::NONE) {
            gr_inventory_player_bar->in_game_scale();
            gr_inventory_player_bar->set_pos(480.f, 648.f);
            return to_return;
        }

        GraphicInventoryRef::check_move_objects(sf::Mouse::getPosition(window), gr_united_inventory_player->gr_items_array);

        window.clear(color);
        window.draw(inventory_screen);
        b_exit.print_button(window);
        show_inventories(window);
        Cursor::move(window);
        window.display();
        play_music();
    }
}

void TradeMenu::bind(std::shared_ptr<GraphicInventoryRef> _gr_inventory_player,
                     std::shared_ptr<GraphicInventoryBar> _gr_inventory_player_bar,
                     std::shared_ptr<GraphicInventoryRef> _gr_united_inventory_player,
                     std::shared_ptr<sf::Text> _gr_money_player) {
    gr_inventory_player = std::shared_ptr<GraphicInventoryRef>(_gr_inventory_player);
    gr_inventory_player_bar = std::shared_ptr<GraphicInventoryBar>(_gr_inventory_player_bar);
    gr_united_inventory_player = std::shared_ptr<GraphicInventoryRef>(_gr_united_inventory_player);
    gr_money_player = std::shared_ptr<sf::Text>(_gr_money_player);
}

void TradeMenu::update_graphic_inventories(const std::vector<std::shared_ptr<Slot>>& items_array_player,
                                           const std::vector<std::shared_ptr<Slot>>& items_array_trader,
                                           std::size_t _money_player, std::size_t _money_trader) {
    gr_inventory_player->set_pos(300.f, 200.f);
    gr_money_player->setPosition({300.f, 130.f});

    for (std::size_t i = 0; i < items_array_player.size() - Inventory::bar_size; i++) {
        if ((*gr_inventory_player)[i]->slot->get_item() != nullptr) {
            float x, y;
            if ((*gr_inventory_player)[i]->slot->get_item()->get_type() == ItemType::WEAPON) {
                x = (*gr_inventory_player)[i]->slot_sprite.getPosition().x;
                y = (*gr_inventory_player)[i]->slot_sprite.getPosition().y;
            } else {
                x = (*gr_inventory_player)[i]->slot_sprite.getPosition().x + 16.f;
                y = (*gr_inventory_player)[i]->slot_sprite.getPosition().y + 16.f;
            }
            (*gr_inventory_player)[i]->slot->get_item()->get_icon().setPosition({x, y});
            (*gr_inventory_player)[i]->slot->get_item()->get_icon().setScale({1.25f, 1.25f});
            (*gr_inventory_player)[i]->gr_amount.setString(
                    std::to_string((*gr_inventory_player)[i]->slot->get_amount()));
            if ((*gr_inventory_player)[i]->slot->get_amount() >= 10 && !(*gr_inventory_player)[i]->gr_amount_offset) {
                x = (*gr_inventory_player)[i]->gr_amount.getPosition().x - 6.f;
                y = (*gr_inventory_player)[i]->gr_amount.getPosition().y;
                (*gr_inventory_player)[i]->gr_amount.setPosition(sf::Vector2f({x, y}));
                (*gr_inventory_player)[i]->gr_amount_offset = true;
            }
        }
    }

    gr_inventory_player_bar->inventory_menu_scale();
    gr_inventory_player_bar->set_pos(424.f, 636.f);

    for (std::size_t i = 0; i < items_array_trader.size(); i++) {
        gr_inventory_trader[i]->slot = items_array_trader[i];
        if (gr_inventory_trader[i]->slot->get_item() != nullptr) {
            float x, y;
            if (gr_inventory_trader[i]->slot->get_item()->get_type() == ItemType::WEAPON) {
                x = gr_inventory_trader[i]->slot_sprite.getPosition().x;
                y = gr_inventory_trader[i]->slot_sprite.getPosition().y;
            } else {
                x = gr_inventory_trader[i]->slot_sprite.getPosition().x + 16.f;
                y = gr_inventory_trader[i]->slot_sprite.getPosition().y + 16.f;
            }
            gr_inventory_trader[i]->slot->get_item()->get_icon().setPosition({x, y});
            gr_inventory_trader[i]->slot->get_item()->get_icon().setScale({1.25f, 1.25f});
            gr_inventory_trader[i]->gr_amount.setString(std::to_string(gr_inventory_trader[i]->slot->get_amount()));
            if (gr_inventory_trader[i]->slot->get_amount() >= 10 && !gr_inventory_trader[i]->gr_amount_offset) {
                x = gr_inventory_trader[i]->gr_amount.getPosition().x - 6.f;
                y = gr_inventory_trader[i]->gr_amount.getPosition().y;
                gr_inventory_trader[i]->gr_amount.setPosition(sf::Vector2f({x, y}));
                gr_inventory_trader[i]->gr_amount_offset = true;
            }
        }
    }

    gr_money_player->setString("$ " + std::to_string(_money_player));
    gr_money_trader.setString("$ " + std::to_string(_money_trader));
}

void TradeMenu::show_inventories(sf::RenderWindow& window) {
    gr_inventory_player->show(window);
    gr_inventory_player_bar->show(window);
    gr_inventory_trader.show(window);
    window.draw(*gr_money_player);
    window.draw(gr_money_trader);
    if (BaseGraphicInventory::chosen_one != BaseGraphicInventory::NONE_CHOSEN) {
        gr_united_inventory_player->gr_items_array[BaseGraphicInventory::chosen_one]->show_slot(window, true);
    }
}

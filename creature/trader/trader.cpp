#include "trader.h"

TradeRequest::TradeRequest() {
    request_spriteL.setTexture(*Resources::TexturesHolder::getResource("main_ui"));
    request_spriteL.setTextureRect({{634, 143}, {24, 24}});
    request_spriteM.setTexture(*Resources::TexturesHolder::getResource("main_ui"));
    request_spriteM.setTextureRect({{659, 143}, {23, 24}});
    request_spriteR.setTexture(*Resources::TexturesHolder::getResource("main_ui"));
    request_spriteR.setTextureRect({{684, 143}, {24, 24}});

    request_text.setFont(*Resources::FontsHolder::getResource("basic_font"));
    request_text.setCharacterSize(12);
    request_text.setOutlineThickness(1);
    request_text.setFillColor(sf::Color(255, 255, 0));
    request_text.setString("TRADE");
}

void TradeRequest::place_request(sf::Vector2f& pos) {
    request_spriteL.setPosition({pos.x, pos.y - 48.f});
    request_spriteM.setPosition({pos.x + 24.f, pos.y - 48.f});
    request_spriteR.setPosition({pos.x + 47.f, pos.y - 48.f});
    request_text.setPosition({pos.x + 14.f, pos.y - 44.f});
}

void TradeRequest::show(sf::RenderWindow& window, sf::Vector2f& pos) {
    place_request(pos);
    window.draw(request_spriteL);
    window.draw(request_spriteM);
    window.draw(request_spriteR);
    window.draw(request_text);
}

Trader::Trader(CreatureManager& _manager, float _health, const sf::Vector2f& _pos, std::mt19937& gen)
        : Creature("man", _manager, _health, _pos, {24.f, 44.f}, {14.f, 14.f}, {32.f, 32.f}) {
    creature_anim = CreatureAnim::HUMANOID;
    creature_type = CreatureType::TRADER;
    set_armor(BodyArmor::make_body(BodyArmorType::BodyArmor_robe));
    set_armor(Helmet::make_helmet(HelmetType::Helmet_robe));
    set_armor(Pants::make_pants(PantsType::Pants_robe_skirt));
    set_armor(Boots::make_boots(BootsType::Boots_brown));
    fill_inventory(gen);
}

void Trader::action(float time, Player* player) {
    update_status(time);
    health_bar.update(health, max_health, hit_box);
    if (dying) {
        Action::dying(this, time);
    } else {
        auto& player_pos = player->get_pos();
        auto diff_x = pos.x - player_pos.x;
        auto diff_y = pos.y - player_pos.y;
        float diff = std::fabs(diff_x) - std::fabs(diff_y);

        if (std::fabs(diff_x) < 48.f && std::fabs(diff_y) < 48.f) {
            request_available = true;
            player->can_accept_request |= true;
            player->available_trader = this;
        } else {
            request_available = false;
            player->can_accept_request |= false;
        }

        if (diff > 0.f) {
            if (diff_x > 0.f) {
                direction = Dirs::LEFT;
            } else {
                direction = Dirs::RIGHT;
            }
        } else {
            if (diff_y > 0.f) {
                direction = Dirs::UP;
            } else {
                direction = Dirs::DOWN;
            }
        }
        Animation::stop_animation(this);
    }
}

void Trader::show_request(sf::RenderWindow& window) {
    if (request_available)
        request.show(window, pos);
}

void Trader::fill_inventory(std::mt19937& gen) {
    inventory.set_capacity(16);
    inventory.set_money(gen() % 90000 + 10000);
    int items_amount = gen() % 12 + 5;
    std::vector<std::shared_ptr<Items>> items;
    for (int i = 0; i < items_amount; ++i) {
        int chance = gen() % 1000;
        std::string id;
        int count;
        if (chance > 200) {
            int food = gen() % 2;
            if (food == 1)
                id = Items::food_ids.common_ids[gen() % Items::food_ids.common_ids.size()];
            else
                id = Items::thing_ids.common_ids[gen() % Items::thing_ids.common_ids.size()];
            count = gen() % 8 + 1;
        } else if (chance > 50) {
            id = Items::thing_ids.uncommon_ids[gen() % Items::thing_ids.uncommon_ids.size()];
            count = gen() % 4 + 1;
        } else if (chance > 10) {
            id = Items::thing_ids.rare_ids[gen() % Items::thing_ids.rare_ids.size()];
            count = gen() % 2 + 1;
        } else {
            id = Items::thing_ids.glorious_ids[gen() % Items::thing_ids.glorious_ids.size()];
            count = 1;
        }
        items.push_back(std::shared_ptr<Items>(new CommonThing(id, count, {0.f, 0.f})));
    }
    while (items.size()) {
        inventory.take(items);
    }
}

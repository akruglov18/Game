#include "Action.h"
#define Left 1
#define Right 2
#define Up 3
#define Down 4

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////MOVEMENTS///////////////////////////////////////////////////////////////

void Action::move_left(Creature* creature, float time, const Field& game_field) {
    auto& current_frame = creature->get_frame();
    current_frame += 0.15f * time;
    auto& pos = creature->get_pos();
    if (game_field(pos.y / 32 + 1, (pos.x - time) / 32 + 1).get_passability())
        pos.x -= time;
    if (current_frame > 8) current_frame = 0;
    creature->get_sprite().setPosition(sf::Vector2f(pos.x, pos.y - 32));
    creature->get_sprite().setTextureRect(sf::IntRect(((int)current_frame + 1) * 64, 64, 64, 64));
    auto body = creature->get_armor().get_body();
    auto helmet = creature->get_armor().get_helmet();
    if(body != nullptr) {
        body->get_sprite().setPosition(sf::Vector2f(pos.x, pos.y - 32));
        body->get_sprite().setTextureRect(sf::IntRect(((int)current_frame + 1) * 64, 64, 64, 64));
    }
    if(helmet != nullptr) {
        helmet->get_sprite().setPosition(sf::Vector2f(pos.x, pos.y - 32));
        helmet->get_sprite().setTextureRect(sf::IntRect(((int)current_frame + 1) * 64, 64, 64, 64));
    }
    creature->get_dir() = Left;
}

void Action::move_right(Creature* creature, float time, const Field& game_field) {
    auto& current_frame = creature->get_frame();
    current_frame += 0.15f * time;
    auto& pos = creature->get_pos();
    if ((game_field(pos.y / 32 + 1, (pos.x + time) / 32 + 1).get_passability()))
        pos.x += time;
    if (current_frame > 8) current_frame = 0;
    creature->get_sprite().setPosition(sf::Vector2f(pos.x, pos.y - 32));
    creature->get_sprite().setTextureRect(sf::IntRect(((int)current_frame + 1) * 64, 192, 64, 64));
    auto body = creature->get_armor().get_body();
    auto helmet = creature->get_armor().get_helmet();
    if (body != nullptr) {
        body->get_sprite().setPosition(sf::Vector2f(pos.x, pos.y - 32));
        body->get_sprite().setTextureRect(sf::IntRect(((int)current_frame + 1) * 64, 192, 64, 64));
    }
    if (helmet != nullptr) {
        helmet->get_sprite().setPosition(sf::Vector2f(pos.x, pos.y - 32));
        helmet->get_sprite().setTextureRect(sf::IntRect(((int)current_frame + 1) * 64, 192, 64, 64));
    }
    creature->get_dir() = Right;
}

void Action::move_up(Creature* creature, float time, const Field& game_field) {
    auto& current_frame = creature->get_frame();
    current_frame += 0.15f * time;
    auto& pos = creature->get_pos();
    if ((game_field((pos.y - time) / 32 + 1, pos.x / 32 + 1).get_passability()))
        pos.y -= time;
    if (current_frame > 8) current_frame = 0;
    creature->get_sprite().setPosition(sf::Vector2f(pos.x, pos.y - 32));
    creature->get_sprite().setTextureRect(sf::IntRect(((int)current_frame + 1) * 64, 0, 64, 64));
    auto body = creature->get_armor().get_body();
    auto helmet = creature->get_armor().get_helmet();
    if(body != nullptr) {
        body->get_sprite().setPosition(sf::Vector2f(pos.x, pos.y - 32));
        body->get_sprite().setTextureRect(sf::IntRect(((int)current_frame + 1) * 64, 0, 64, 64));
    }
    if(helmet != nullptr) {
        helmet->get_sprite().setPosition(sf::Vector2f(pos.x, pos.y - 32));
        helmet->get_sprite().setTextureRect(sf::IntRect(((int)current_frame + 1) * 64, 0, 64, 64));
    }
    creature->get_dir() = Up;
}

void Action::move_down(Creature* creature, float time, const Field& game_field) {
    auto& current_frame = creature->get_frame();
    current_frame += 0.15f * time;
    current_frame += 0.15f * time;
    auto& pos = creature->get_pos();
    if ((game_field((pos.y + time) / 32 + 1, pos.x / 32 + 1).get_passability()))
        pos.y += time;
    if (current_frame > 8) current_frame = 0;
    creature->get_sprite().setPosition(sf::Vector2f(pos.x, pos.y - 32));
    creature->get_sprite().setTextureRect(sf::IntRect(((int)current_frame + 1) * 64, 128, 64, 64));
    auto body = creature->get_armor().get_body();
    auto helmet = creature->get_armor().get_helmet();
    if(body != nullptr) {
        body->get_sprite().setPosition(sf::Vector2f(pos.x, pos.y - 32));
        body->get_sprite().setTextureRect(sf::IntRect(((int)current_frame + 1) * 64, 128, 64, 64));
    }
    if(helmet != nullptr) {
        helmet->get_sprite().setPosition(sf::Vector2f(pos.x, pos.y - 32));
        helmet->get_sprite().setTextureRect(sf::IntRect(((int)current_frame + 1) * 64, 128, 64, 64));
    }
    creature->get_dir() = Down;
}

void Action::stop(Creature* creature) {
    // It's possible to add some micro left-right movements to imitate breath while staying, but I don't want to
    auto dir = creature->get_dir();
    auto body = creature->get_armor().get_body();
    auto helmet = creature->get_armor().get_helmet();;
    switch (dir) {
    case(Left):
        creature->get_sprite().setTextureRect(sf::IntRect(0, 64, 64, 64));
        if(body != nullptr)
            body->get_sprite().setTextureRect(sf::IntRect(0, 64, 64, 64));
        if(helmet != nullptr)
            helmet->get_sprite().setTextureRect(sf::IntRect(0, 64, 64, 64));
        break;
    case(Right):
        creature->get_sprite().setTextureRect(sf::IntRect(0, 192, 64, 64));
        if(body != nullptr)
            body->get_sprite().setTextureRect(sf::IntRect(0, 192, 64, 64));
        if(helmet != nullptr)
            helmet->get_sprite().setTextureRect(sf::IntRect(0, 192, 64, 64));
        break;
    case(Up):
        creature->get_sprite().setTextureRect(sf::IntRect(0, 0, 64, 64));
        if(body != nullptr)
            body->get_sprite().setTextureRect(sf::IntRect(0, 0, 64, 64));
        if(helmet != nullptr)
            helmet->get_sprite().setTextureRect(sf::IntRect(0, 0, 64, 64));
        break;
    case(Down):
        creature->get_sprite().setTextureRect(sf::IntRect(0, 128, 64, 64));
        if(body != nullptr)
            body->get_sprite().setTextureRect(sf::IntRect(0, 128, 64, 64));
        if(helmet != nullptr)
            helmet->get_sprite().setTextureRect(sf::IntRect(0, 128, 64, 64));
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////ANOTHER ACTIONS/////////////////////////////////////////////////////////////
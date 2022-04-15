#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "field.h"
#include "creature.h"
#include "animation.h"
#include "utils.h"

class Action
{
public:
    // movements
    static void move_left(Creature* creature, float time, const std::shared_ptr<Field>& game_field);
    static void move_right(Creature* creature, float time, const std::shared_ptr<Field>& game_field);
    static void move_up(Creature* creature, float time, const std::shared_ptr<Field>& game_field);
    static void move_down(Creature* creature, float time, const std::shared_ptr<Field>& game_field);

    // another actions such as take, cut, hit, trade, use magic and so on
    static void hit(Creature* creature, float time, const std::vector<std::shared_ptr<Creature>>& drawable_creatures);
    static void dying(Creature* creature, float time);

    // animation
    static void update_frame(Creature* creature, float time);

private:
    // inner, additional functions
    static void make_borders(Creature* creature, float& top_hit_border, float& btm_hit_border, float& left_hit_border, float& right_hit_border);
    static void choose_mode_according_to_weapon(Creature* creature);

};


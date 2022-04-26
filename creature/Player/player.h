#pragma once
#include "armor_set.h"
#include "creature.h"
#include "field.h"
#include "inventory.h"

class Player : public Creature {
public:
    Player(CreatureManager& manager, int health = 100, const sf::Vector2f& pos = {256, 256});
    void action(sf::Event& event, float time, Field* game_field,
                const std::vector<std::shared_ptr<Creature>>& drawable_creatures);

    Inventory inventory;

private:
    void take_drop(Field* field);
};

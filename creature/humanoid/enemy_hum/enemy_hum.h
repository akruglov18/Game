#pragma once
#include "humanoid/humanoid.h"

class Enemy_hum : public Humanoid {
public:
    Enemy_hum(const std::string& name, CreatureManager& manager, int health, const sf::Vector2f& pos);
    static std::shared_ptr<Enemy_hum> spawn_enemy(CreatureType, CreatureManager&, int health, const sf::Vector2f& pos);
    void action(float time);
};
#pragma once

#include "base_weapon.h"

enum class SpearType { Spear_wood };

class Spear : public BaseWeapon {
public:
    Spear(const std::string& name, SpearType type);
    static std::shared_ptr<Spear> make_spear(SpearType type);
    static std::shared_ptr<Spear> make_spear_from_json(const json& json_obj);
    void calculate_damage_box(sf::Vector2f& pos, int dir, Modes mode) override;
};

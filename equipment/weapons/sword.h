#pragma once

#include "base_weapon.h"

enum class SwordType { Long_sword_basic };

class Sword : public BaseWeapon {
public:
    Sword(const std::string& name, SwordType type);
    static std::shared_ptr<Sword> make_sword(SwordType type);
    static std::shared_ptr<Sword> make_sword_from_json(const json& json_obj);
    void calculate_damage_box(sf::Vector2f& pos, int dir, Modes mode) override;
};

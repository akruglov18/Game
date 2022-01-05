#pragma once

#include <items.h>
#include "sfml/Graphics.hpp"
#include <vector>
#include "ResourceHolder.h"

enum class ArmorType{
    HELMET,
    BODY_ARMOR,
    GAUNTLETS,
    PANTS,
    BOOTS
};

class BaseArmor : public Items {
public:
    BaseArmor(const std::string& name, const sf::Vector2f& pos);

    sf::Sprite& get_sprite() { return _sprite; }
    std::vector<std::shared_ptr<sf::Texture>>& get_textures() { return _textures; }

    static void change_mode(Modes mode, std::shared_ptr<BaseArmor>);

protected:
    int _armor = 0;
    ArmorType _armor_type;
    /*
    int _added_health;
    ...
    */
};
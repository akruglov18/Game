#pragma once

#include <random>
#include <items.h>

enum class WeaponType {
    SWORD,
    LONG_SWORD,
    SPEAR,
    KNIFE,
    AXE
};

class Weapon : public Items {
public:
    Weapon();
    //Weapon(const Weapon& other);

    virtual double get_damage() const;
    double damage() const { return _damage; }
    int critical_chance() const { return _critical_chance; }
    WeaponType get_weapon_type() const { return _weapon_type; }

    sf::Sprite& get_sprite() { return _sprite; }

    void walk();
    void thrust();
    void spellcast();
    void slash();
    void hurt();
    void bow();

    virtual ~Weapon(){}
protected:
    std::vector<std::shared_ptr<sf::Texture>> _textures;
    sf::Sprite _sprite;
    double _damage = 0;
    int _critical_chance = 0;
    double _critical_multiplier = 0;
    WeaponType _weapon_type;
};

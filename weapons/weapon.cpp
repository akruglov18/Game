#include "weapon.h"

static auto HOLDER = getGlobalResourceHolder<sf::Texture, std::string>;

Weapon::Weapon(const std::string& name, const sf::Vector2f& pos) {
    item_type = ItemType::WEAPON;
    textures.resize(static_cast<int>(Modes::MODES_SIZE));
    for (int i = 0; i < static_cast<int>(Modes::MODES_SIZE); ++i) {
        textures[i] = HOLDER().getResource(name + suffixes[i]);
    }
    init_scale(pos);
}

Weapon::Weapon(const Weapon& other) {
    textures = other.textures;
    sprite = other.sprite;
    damage = other.damage;
    critical_chance = other.critical_chance;
    critical_multiplier = other.critical_multiplier;
    weapon_type = other.weapon_type;
}

double Weapon::get_total_damage() const {
    std::random_device device;
    std::mt19937 gen(device());
    int chance = gen() % 101;
    double result = damage;
    if (chance <= critical_chance) {
        result = damage*critical_multiplier;
    }
    return result;
}

json Weapon::to_json() const {
    json res;
    auto name = std::string("Weapon");
    res[name]["class_type"] = string_type();
    res[name]["damage"] = damage;
    res[name]["critical_chance"] = critical_chance;
    res[name]["critical_multiplier"] = critical_multiplier;
    return res;
}

void Weapon::change_mode(Modes mode) {
    sprite.setTexture(*textures[static_cast<int>(mode)]);
}

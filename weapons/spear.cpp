#include "spear.h"

static auto HOLDER = getGlobalResourceHolder<sf::Texture, std::string>;

Spear::Spear() : Weapon() {
    _weapon_type = WeaponType::SPEAR;
    _critical_chance = 40;
    _critical_multiplier = 1.1;
}

Spear_wood::Spear_wood(const sf::Vector2f& pos) {
    for (int i = 0; i < T_SIZE; ++i) {
        _textures[i] = HOLDER().getResource("SPEAR_wood" + _suffixes[i]);
    }
    _damage = 10.0;
    _sprite.setTexture(*_textures[0]);
    _sprite.setTextureRect(sf::IntRect(0, 128, 64, 64));
    _sprite.setPosition(sf::Vector2f(pos.x, pos.y - 32));
}
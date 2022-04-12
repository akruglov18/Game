#include "beetle.h"

Beetle::Beetle(CreatureManager& _manager, int _health, const sf::Vector2f& _pos)
        : Enemy("NONE", _manager, _health, _pos, {32.f, 32.f}, {16.f, 8.f}) {
    creature_type = CreatureType::BEETLE;
}

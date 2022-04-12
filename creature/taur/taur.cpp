#include "taur.h"

Taur::Taur(CreatureManager& _manager, int _health, const sf::Vector2f& _pos): 
    Enemy("NONE", _manager, _health, _pos, {32.f, 46.f}, {24.f, 16.f}) {
    creature_type = CreatureType::TAUR;
}

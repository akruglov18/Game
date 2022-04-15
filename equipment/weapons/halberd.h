#pragma once
#include "base_weapon.h"

class Halberd : public BaseWeapon {
public:
    Halberd(const std::string& name);
    static std::shared_ptr<Halberd> make_halberd();
    static std::shared_ptr<Halberd> make_halberd_from_json(const json& json_obj);
};
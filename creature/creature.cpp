#include "creature.h"
#include "Player/player.h"
#include "ResourceHolder.h"
#include "enemy.h"

#define STUCK_TIME 3.f

Creature::Creature(const std::string& _name, CreatureManager& _manager, int _health, const sf::Vector2f& _pos,
                   const sf::Vector2f& _hit, const sf::Vector2f& _collision, const sf::Vector2f& _centre_offset)
        : manager(_manager),
          pos(_pos),
          hit_box({{_pos.x + _centre_offset.x - _hit.x / 2.f, _pos.y + _centre_offset.y - _hit.y}, _hit}),
          collision_box({_pos.x + _centre_offset.x - _collision.x / 2.f, _pos.y + _centre_offset.y - _collision.y}, _collision),
          centre_offset(_centre_offset) {

    rect_hit_box.setOutlineThickness(2.f);
    rect_hit_box.setOutlineColor(sf::Color(255, 0, 0));
    rect_hit_box.setFillColor(sf::Color(255, 255, 255, 0));
    rect_hit_box.setPosition(hit_box.getPosition());
    rect_hit_box.setSize(hit_box.getSize());

    rect_collision_box.setOutlineThickness(2.f);
    rect_collision_box.setOutlineColor(sf::Color(0, 0, 255));
    rect_collision_box.setFillColor(sf::Color(255, 255, 255, 0));
    rect_collision_box.setPosition(collision_box.getPosition());
    rect_collision_box.setSize(collision_box.getSize());

    current_frame = 0.f;
    experience = 0;
    health = _health;
    direction = Dirs::DOWN;
    mode = Modes::WALK;
    body_textures.resize(static_cast<int>(Modes::MODES_SIZE));
    for (int i = 0; i < static_cast<int>(Modes::MODES_SIZE); ++i) {
        body_textures[i] = Resources::TexturesHolder::getResource(_name + Items::suffixes[i]);
    }
    sprite.setTexture(*body_textures[static_cast<int>(Modes::WALK)]);
    sprite.setTextureRect(sf::IntRect({0, 128}, {64, 64}));
    sprite.setPosition(sf::Vector2f(_pos.x, _pos.y - 32));
}

Creature::Creature(const Creature& other): manager(other.manager), pos(other.pos) {
    current_frame = other.current_frame;
    health = other.health;
    body_textures = other.body_textures;
    sprite = other.sprite;
    creature_type = other.creature_type;
    direction = other.direction;
    action_animation_duration = other.action_animation_duration;
    mode = other.mode;
}

void Creature::set_pos(float x, float y) {
    pos.x = x;
    pos.y = y;
}

void Creature::set_armor(std::shared_ptr<BaseArmor> armor) {
    armor_set[armor->get_armor_type()] = armor;
    armor->set_scale(pos);
}

void Creature::set_weapon(std::shared_ptr<BaseWeapon> _weapon) {
    weapon = _weapon;
    weapon->set_scale(pos);
}

void Creature::set_health(int _health) {
    health = _health;
}

int Creature::get_damage() const {
    if (weapon == nullptr)
        return 0;
    return static_cast<int>(weapon->get_total_damage());
}

void Creature::become_red() {
    sprite.setColor(sf::Color(250, 0, 0, 240));
    for (auto& el : armor_set.armor_set) {
        if (el != nullptr)
            el->get_sprite().setColor(sf::Color(250, 0, 0, 240));
    }
}

void Creature::become_normal() {
    sprite.setColor(sf::Color(255, 255, 255));
    for (auto& el : armor_set.armor_set) {
        if (el != nullptr)
            el->get_sprite().setColor(sf::Color(255, 255, 255));
    }
}

void Creature::reduce_health(int value) {
    health -= value;
    // std::cout << "health = " << health << '\n';
    stuck = true;
    stuck_time = STUCK_TIME;
    if (health <= 0) {
        died = true;
    }
}

void Creature::update_stuck_frame(float time) {
    if (stuck) {
        stuck_time -= 0.15f * time;
        if (stuck_time < 0) {
            stuck = false;
            stuck_time = 0.f;
        }
    }
}

void Creature::die() {
    manager.creatureDied(this);
}

void Creature::add_experience(int exp) {
    experience += exp;
}

void Creature::show_creature(sf::RenderWindow& window) {
    window.draw(sprite);
    if (direction == Dirs::UP)
        window.draw(get_weapon()->get_sprite());

    for (auto& el : get_armor().armor_set) {
        if (el != nullptr)
            window.draw(el->get_sprite());
    }

    if (get_weapon() != nullptr && direction != Dirs::UP)
        window.draw(get_weapon()->get_sprite());
}

void Creature::show_box(sf::RenderWindow& window) {
    window.draw(rect_hit_box);
    window.draw(rect_collision_box);
}

std::string Creature::creature_type_str() const {
    switch (creature_type) {
    case CreatureType::PLAYER:
        return "Player";
    case CreatureType::BEETLE:
        return "Beetle";
    case CreatureType::WOLF:
        return "Wolf";
    case CreatureType::TRADER:
        return "Trader";
    case CreatureType::TAUR:
        return "Taur";
    case CreatureType::SKELETON:
        return "Skeleton";
    case CreatureType::NONE:
        return "NONE";
    case CreatureType::SPIDER:
        return "SPIDER";
    default:
        throw std::logic_error("Invalid creature type");
    }
}

json Creature::to_json() const {
    json res;
    res["health"] = health;
    res["experience"] = experience;
    res["pos"]["x"] = pos.x;
    res["pos"]["y"] = pos.y;
    res[armor_set.class_name()] = armor_set.to_json();
    if (weapon != nullptr)
        res[BaseWeapon::class_name()] = weapon->to_json();
    return res;
}

void Creature::load(const json& json_obj) {
    for (auto it = json_obj.begin(); it != json_obj.end(); ++it) {
        if (it.key() == "health") {
            health = it.value();
        } else if (it.key() == "experience") {
            experience = it.value();
        } else if (it.key() == "pos") {
            pos.x = it.value()["x"];
            pos.y = it.value()["y"];
        } else if (it.key() == ArmorSet::class_name()) {
            armor_set.load(it.value());
        } else if (it.key() == BaseWeapon::class_name()) {
            weapon = BaseWeapon::load(it.value());
        } else {
            throw std::invalid_argument("Unused key in json save: " + it.key());
        }
    }
}

void Creature::change_mode(Modes _mode) {
    sprite.setTexture(*body_textures[static_cast<int>(_mode)]);
    armor_set.change_mode(_mode);
    if (weapon != nullptr)
        weapon->change_mode(_mode);
    mode = _mode;
}

void CreatureManager::setPlayer(Player* _player) {
    player = _player;
}

void CreatureManager::setEnemies(std::vector<std::shared_ptr<Enemy>>* _enemies) {
    enemies = _enemies;
}

void CreatureManager::creatureDied(Creature* creature) {
    if (creature->get_type() == CreatureType::NONE)
        throw std::logic_error("Creature died, Creture type: NONE");

    creature->to_delete_from_vector = true;

    if (creature->get_type() == CreatureType::PLAYER) {
        //end
    } else {
        player->add_experience(10);    
    }

}

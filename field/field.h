#pragma once
#include <chrono>
#include <iomanip>
#include <numeric>
#include "tiles.h"

class Field {
private:
    int _width;
    int _height;
    std::vector<std::vector<std::shared_ptr<Tile>>> _field;

public:
    // constructors~destructor
    Field();
    Field(int w, int h);
    Field(Field&&) noexcept;
    Field(const Field&) = delete;
    ~Field() {
    }

    // operators
    Field& operator=(Field&&) noexcept;
    std::shared_ptr<Tile> operator()(int i, int j) const;
    Field& operator=(const Field&) = delete;

    // getters & setters
    int get_width() const {
        return _width;
    }
    int get_height() const {
        return _height;
    }
    void set_size(int widht, int height);

    // sprites
    sf::Sprite sand1_sprite;
    sf::Sprite sand1_borders_sprite;
    sf::Sprite dry1_sprite;
    sf::Sprite desert_feature_sprite;
    sf::Sprite desert_tree_sprite;

    // Bioms generation
    void generate_desert();
    void generate_savanna();
    void generate_grassland();
    void generate_snowland();

    void generate_rivers();
    void generate_lakes();

    void generate_trees();
    void generate_roads();
    void generate_buildings();

    void generate_treasures();
    void generate_trade_stores();

    void generate_field();
};

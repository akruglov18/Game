#include "field.h"
#include <iostream>
#include <random>
#include "all_tiles.h"

// constructors~destructor /////////////////////////////////////////////////////////////////////////////////////////////

Field::Field() {
    _width = 16;
    _height = 16;
    _field.assign(_height, std::vector<std::shared_ptr<Tile>>(_width, nullptr));
}

Field::Field(int width, int height) {
    _width = width;
    _height = height;
    _field.assign(_height, std::vector<std::shared_ptr<Tile>>(_width, nullptr));
    sand1_sprite.setTexture(*Resources::TexturesHolder::getResource("sand1"));
    sand1_borders_sprite.setTexture(*Resources::TexturesHolder::getResource("borders_sand1"));
    dry1_sprite.setTexture(*Resources::TexturesHolder::getResource("dry1"));
    desert_feature_sprite.setTexture(*Resources::TexturesHolder::getResource("desert_features"));
    desert_tree_sprite.setTexture(*Resources::TexturesHolder::getResource("desert_trees"));
}

Field::Field(Field&& field) noexcept: _field(std::move(field._field)) {
    _width = field._width;
    _height = field._height;
    field._width = 0;
    field._height = 0;
}

// operators //////////////////////////////////////////////////////////////////////////////////////////////////////////

Field& Field::operator=(Field&& field) noexcept {
    if (this == &field)
        return *this;
    _field = std::move(field._field);
    _width = field._width;
    _height = field._height;
    field._width = 0;
    field._height = 0;
    return *this;
}

std::shared_ptr<Tile> Field::operator()(int i, int j) const {
    return _field[i][j];
}

// methods /////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Field::set_size(int width, int height) {
    _width = width;
    _height = height;
    _field.resize(_height);
    for (int i = 0; i < _height; ++i) {
        _field[i].resize(_width);
    }
}

void Field::generate_desert() {
    // GENERATION OF LANDSCAPE
    std::mt19937 gen;
    gen.seed(static_cast<unsigned>(time(0)));

    // Borders
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < _width; ++j) {
            _field[i][j] = std::shared_ptr<Desert_borders>(new Desert_borders(sand1_borders_sprite));
            _field[static_cast<std::size_t>(i + _height - 4)][j] =
                    std::shared_ptr<Desert_borders>(new Desert_borders(sand1_borders_sprite));
            _field[j][i] = std::shared_ptr<Desert_borders>(new Desert_borders(sand1_borders_sprite));
            _field[j][static_cast<std::size_t>(i + _width - 4)] =
                    std::shared_ptr<Desert_borders>(new Desert_borders(sand1_borders_sprite));
        }
    }

    // Desert cracks and palms
    for (int i = 16; i < _height - 16; i += 48) {
        for (int j = 16; j < _width - 16; j += 48) {
            int height = gen() % 6 + 12;
            int start_x = gen() % 13 - 6 + j;
            int start_y = gen() % 13 - 6 + i;
            int dir = -1 + 2 * (gen() & 1);
            while (height--) {
                int width = gen() % 5 + 6 - height / 4;
                if (height < 5)
                    width -= gen() % 3 + 3;
                for (int k = start_x; k < start_x + width; ++k) {
                    _field[start_y][k] = std::shared_ptr<Desert_cracks>(new Desert_cracks(dry1_sprite));
                    if (_field[static_cast<std::size_t>(start_y - 1)][k] != nullptr &&
                        _field[start_y][static_cast<std::size_t>(k - 1)] != nullptr && gen() % 8 == 0) {
                        if (!_field[static_cast<std::size_t>(start_y - 1)][k]->tree &&
                            !_field[start_y][static_cast<std::size_t>(k - 1)]->tree) {
                            _field[start_y][k]->tree = gen() % 6 + 1;
                            _field[start_y][k]->get_passability() = 0;
                        }
                    }
                }
                start_y++;
                start_x += dir * gen() % 3;
            }
        }
    }

    // Sand & little features
    for (int i = 4; i < _height - 4; ++i) {
        for (int j = 4; j < _width - 4; ++j) {
            if (_field[i][j] == nullptr) {
                _field[i][j] = std::shared_ptr<Desert_sand>(new Desert_sand(sand1_sprite));
                if (gen() % 16 == 0) {
                    _field[i][j]->feature = gen() % 6 + 1;
                }
            }
        }
    }
}

// It seems to be big and sophisticated method of generation of the game field.
void Field::generate_field() {
    /////////////////////////////////////////////////////////////TIME_CHECK///////////////////////////////////////////////////////////
    std::cout << std::fixed << std::setprecision(9) << std::left;
    auto start = std::chrono::high_resolution_clock::now();
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    generate_desert();

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Generate field: " << std::setw(9) << diff.count() << " s\n";
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

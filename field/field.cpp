#include "field.h"
#include <random>

// constructors~destructor /////////////////////////////////////////////////////////////////////////////////////////////

Field::Field() {
    _width = 16;
    _height = 16;
    _field.assign(_height, std::vector<Tile*>(_width, nullptr));
}

Field::Field(int width, int height) {
    _width = width;
    _height = height;
    _field.assign(_height, std::vector<Tile*>(_width, nullptr));
}

Field::Field(Field&& field) : _field(std::move(field._field)) {
    _width = field._width;
    _height = field._height;
    field._width = 0;
    field._height = 0;
}

Field::~Field() {
    for (int i = 0; i < _height; ++i) {
        for (int j = 0; j < _width; ++j) {
            delete _field[i][j];
            _field[i][j] = nullptr;
        }
    }
}

// operators //////////////////////////////////////////////////////////////////////////////////////////////////////////

Field& Field::operator=(Field&& field) {
    _field = std::move(field._field);
    _width = field._width;
    _height = field._height;
    field._width = 0;
    field._height = 0;
    return *this;
}

Tile Field::operator()(int i, int j) const {
    return *_field[i][j];
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
    auto HOLDER = getGlobalResourceHolder<sf::Texture, std::string>;
    
    // GENERATION OF LANDSCAPE
    std::mt19937 gen;
    gen.seed(time(0));
    std::vector<std::vector<char>> temp(_height, std::vector<char>(_width, 's'));

    // Borders
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < _width; ++j) {
            temp[i][j] = 'B';
            temp[i + _height - 4][j] = 'B';
        }
    }
    for (int i = 0; i < _height; ++i) {
        for (int j = 0; j < 4; ++j) {
            temp[i][j] = 'B';
            temp[i][j + _width - 4] = 'B';
        }
    }
    
    // Oases
    for (int i = 8; i < _height - 8; i += 16) {
        for (int j = 8; j < _width - 8; j += 16) {
            int oasis_chance = gen() % 2;
            if (oasis_chance) {
                int x_chance = gen() % 4;
                int y_chance = gen() % 4;
                for (int p = i + y_chance * 4; p < i + y_chance * 4 + 4; ++p) {
                    for (int q = j + x_chance * 4; q < j + x_chance * 4 + 4; ++q) {
                        temp[p][q] = 'O';
                    }
                }
            }
        }
    }

    // TEXTURING
    for (int i = 0; i < _height; ++i) {
        for (int j = 0; j < _width; ++j) {
            switch (temp[i][j]) {
                case('s'):
                    _field[i][j] = Tile::make_tile(TilesType::DESERT, HOLDER().getResource("sand1")); break;
                case('O'):
                    _field[i][j] = Tile::make_tile(TilesType::DESERT, HOLDER().getResource("oasis1")); break;
                case('B'):
                    _field[i][j] = Tile::make_tile(TilesType::DESERT, HOLDER().getResource("borders_sand1")); break;
            }
            if (temp[i][j] == 'B')
                _field[i][j]->scale_borders(i, j, _width, _height);
            else
                _field[i][j]->scale(i, j);
            _field[i][j]->get_sprite().move(sf::Vector2f(j * 32, i * 32));
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

void Field::show_field(sf::RenderWindow& window, const sf::Vector2f& pos) {
    int left_border, right_border, top_border, btm_border;
    int window_height = window.getSize().y;
    int window_width = window.getSize().x;
    int tile_size = 32; // immutable parameter

    // borders of rendering ///////////////////////////////////////////////////////////////////////////////////
    if (pos.x < window_width / 2)
        right_border = window_width / tile_size + 2;
    else
        right_border = std::min(static_cast<int>(_field[0].size()), static_cast<int>(((pos.x + window_width / 2) / tile_size) + 1));
    if (pos.x > _field[0].size() * tile_size - window_width / 2)
        left_border = _field[0].size() - window_width / tile_size - 2;
    else
        left_border = std::max(0, static_cast<int>(((pos.x - window_width / 2) / tile_size)));
    if (pos.y < window_height / 2)
        btm_border = window_height / tile_size + 2;
    else
        btm_border = std::min(static_cast<int>(_field.size()), static_cast<int>(((pos.y + window_height / 2) / tile_size) + 1));
    if (pos.y > _field.size() * tile_size - window_height / 2)
        top_border = _field.size() - window_height / tile_size - 2;
    else
        top_border = std::max(0, static_cast<int>(((pos.y - window_height / 2) / tile_size)));
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    for (int i = top_border; i < btm_border; ++i) {
        for (int j = left_border; j < right_border; ++j) {
            window.draw(_field[i][j]->print_tile());
        }
    }
}

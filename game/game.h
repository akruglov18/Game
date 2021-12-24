#pragma once
#include "field.h"
#include "creature.h"

class Game {
private:
	sf::RenderWindow _window{ sf::VideoMode(1280, 720), "Consol game"};
	int _size = 128;
	Field _game_field{_size, _size};
	int _game_region_width = _size * 32; // size in pixels
	int _game_region_height = _size * 32;
	std::shared_ptr<Creature> _player;
	std::vector<Creature*> enemies;
    CreatureManager _manager;
	sf::View _view;

public:
	Game();

	// Main loop
	void game_loop();

	// Game methods
	void load_textures();
	void render();
	sf::View get_player_pos_for_view(const sf::Vector2f&);
};
#include "field.h"

Field::Field(std::size_t _size)
{
	field.assign(_size, std::vector<Tile>(_size));
}

// It's supposed to be really big and sophisticated function of generation of game field, but now it's absolutely random.
void Field::generate_field()
{
	for (int i = 0; i < field.size(); ++i) {
		for (int j = 0; j < field.size(); ++j) {
			std::random_device device;
			std::mt19937 gen(device());
			int chance = gen() % 6 + 1;
			field[i][j].set_type(chance);
		}
	}
	
}

void Field::show_field()
{
	for (int i = 0; i < field.size(); ++i) {
		for (int j = 0; j < field.size(); ++j) {
			SetConsoleTextAttribute(hConsole, (WORD)((3 << 4) | field[i][j].get_color()));
			std::cout << field[i][j].get_value();
			SetConsoleTextAttribute(hConsole, (WORD)((3 << 4) | 0));
		}
	}
}

std::ostream& operator<< (std::ostream& out, const Field& field)
{
	throw 0;
}